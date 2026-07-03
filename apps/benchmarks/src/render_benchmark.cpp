#include <htracer_benchmarks/render_benchmark.hpp>

#include <htracer/htracer.hpp>
#include <htracer_benchmarks/model.hpp>
#include <htracer_benchmarks/scenes.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>


namespace htracer::benchmarks
{

namespace
{

constexpr std::uint64_t fnv_offset_basis{14695981039346656037ULL};
constexpr std::uint64_t fnv_prime{1099511628211ULL};


void
hash_bytes(std::uint64_t &hash, std::span<std::byte const> bytes) noexcept
{
  for (auto const byte : bytes)
  {
    hash ^= std::to_integer<std::uint8_t>(byte);
    hash *= fnv_prime;
  }
}


template<typename T>
void
hash_value(std::uint64_t &hash, T const &value) noexcept
{
  hash_bytes(hash, std::as_bytes(std::span{&value, std::size_t{1}}));
}


template<std::floating_point Float>
[[nodiscard]]
std::uint64_t
validate_and_hash_image(
    typename htracer::float_traits<Float>::image const &image, render_configuration const &configuration)
{
  if (image.h_res() != configuration.width || image.v_res() != configuration.height)
  {
    throw std::runtime_error("render returned unexpected image dimensions");
  }

  auto const expected_pixels = static_cast<std::size_t>(configuration.width) * configuration.height;
  if (image.pixels().size() != expected_pixels)
  {
    throw std::runtime_error("render returned unexpected pixel count");
  }

  std::uint64_t hash = fnv_offset_basis;
  hash_value(hash, configuration.width);
  hash_value(hash, configuration.height);

  for (auto const &pixel : image.pixels())
  {
    for (std::size_t channel = 0; channel < 3; ++channel)
    {
      auto const value = pixel[channel];
      if (!std::isfinite(value))
      {
        throw std::runtime_error("render returned a non-finite pixel channel");
      }
      hash_value(hash, value);
    }
  }

  return hash;
}


void
combine_hash(std::uint64_t &run_hash, std::uint64_t image_hash) noexcept
{
  hash_value(run_hash, image_hash);
}


[[nodiscard]]
duration_summary
summarize(std::vector<std::uint64_t> const &samples)
{
  if (samples.empty())
  {
    throw std::runtime_error("cannot summarize an empty measurement");
  }

  auto sorted = samples;
  std::ranges::sort(sorted);

  auto const middle = sorted.size() / 2;
  auto median = sorted[middle];
  if (sorted.size() % 2 == 0)
  {
    auto const lower = sorted[middle - 1];
    auto const upper = sorted[middle];
    median = lower + (upper - lower) / 2;
  }

  if (median == 0)
  {
    throw std::runtime_error("measured median duration is zero; use a larger workload");
  }

  return {.minimum_ns = sorted.front(), .median_ns = median, .maximum_ns = sorted.back()};
}


template<std::floating_point Float, typename RenderOnce>
[[nodiscard]]
benchmark_result
measure(benchmark_case const &benchmark, RenderOnce const &render_once)
{
  std::uint64_t run_hash = fnv_offset_basis;

  for (std::uint32_t warmup = 0; warmup < benchmark.measurement.warmup_count; ++warmup)
  {
    auto const image = render_once();
    combine_hash(run_hash, validate_and_hash_image<Float>(image, benchmark.render));
  }

  std::vector<std::uint64_t> samples;
  samples.reserve(benchmark.measurement.repetition_count);
  std::optional<std::uint64_t> expected_image_hash;
  auto const reproducible = benchmark.render.rendering == rendering_kind::deterministic || benchmark.render.seed;

  for (std::uint32_t repetition = 0; repetition < benchmark.measurement.repetition_count; ++repetition)
  {
    auto const started_at = std::chrono::steady_clock::now();
    auto const image = render_once();
    auto const stopped_at = std::chrono::steady_clock::now();

    auto const elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(stopped_at - started_at).count();
    if (elapsed < 0)
    {
      throw std::runtime_error("steady clock produced a negative duration");
    }
    samples.push_back(static_cast<std::uint64_t>(elapsed));

    auto const image_hash = validate_and_hash_image<Float>(image, benchmark.render);
    combine_hash(run_hash, image_hash);

    if (reproducible)
    {
      if (expected_image_hash && *expected_image_hash != image_hash)
      {
        throw std::runtime_error("deterministic or seeded render changed across repetitions");
      }
      expected_image_hash = image_hash;
    }
  }

  auto const summary = summarize(samples);
  return {.benchmark = benchmark, .samples_ns = std::move(samples), .summary = summary, .checksum = run_hash};
}


template<std::floating_point Float, typename Renderer, typename Scene, htracer::rendering::rendering_policy Policy>
[[nodiscard]]
benchmark_result
measure_deterministic(benchmark_case const &benchmark, Renderer const &renderer, Scene const &scene, Policy policy)
{
  return measure<Float>(benchmark, [&renderer, &scene, policy]() { return renderer.render(policy, scene); });
}


template<std::floating_point Float, typename Renderer, typename Scene, htracer::rendering::rendering_policy Policy>
[[nodiscard]]
benchmark_result
measure_randomized(benchmark_case const &benchmark, Renderer const &renderer, Scene const &scene, Policy policy)
{
  if (!benchmark.render.samples_per_pixel)
  {
    throw std::invalid_argument("randomized benchmark is missing samples per pixel");
  }

  auto const samples = htracer::rendering::samples_per_pixel{*benchmark.render.samples_per_pixel};
  if (benchmark.render.seed)
  {
    auto const seed = htracer::rendering::random_seed{*benchmark.render.seed};
    return measure<Float>(
        benchmark,
        [&renderer, &scene, policy, samples, seed]() { return renderer.render(policy, scene, samples, seed); });
  }

  return measure<Float>(
      benchmark, [&renderer, &scene, policy, samples]() { return renderer.render(policy, scene, samples); });
}


template<std::floating_point Float>
[[nodiscard]]
benchmark_result
run_typed(benchmark_case const &benchmark)
{
  using traits = htracer::float_traits<Float>;

  if (benchmark.render.batcher != batcher_kind::column || benchmark.render.lens != lens_kind::pinhole)
  {
    throw std::invalid_argument("unsupported MVP renderer components");
  }

  auto setup = make_scene<Float>(benchmark.render);
  typename traits::camera const camera{
      setup.camera_position,
      setup.camera_view,
      setup.camera_up,
      benchmark.render.width,
      benchmark.render.height,
      setup.fov};
  htracer::rendering::batchers::column_batcher const batcher;
  typename traits::pinhole_lens const lens;

  if (benchmark.render.rendering == rendering_kind::deterministic)
  {
    if (benchmark.render.sensor != sensor_kind::point)
    {
      throw std::invalid_argument("deterministic MVP benchmark requires point sensor");
    }

    typename traits::point_sensor const sensor;
    auto const renderer = htracer::rendering::make_renderer(camera, batcher, sensor, lens);
    if (benchmark.render.policy == policy_kind::seq)
    {
      return measure_deterministic<Float>(benchmark, renderer, setup.scene, htracer::rendering::seq);
    }
    return measure_deterministic<Float>(benchmark, renderer, setup.scene, htracer::rendering::par);
  }

  if (benchmark.render.sensor != sensor_kind::uniform)
  {
    throw std::invalid_argument("randomized MVP benchmark requires uniform sensor");
  }

  typename traits::uniform_sensor const sensor;
  auto const renderer = htracer::rendering::make_renderer(camera, batcher, sensor, lens);
  if (benchmark.render.policy == policy_kind::seq)
  {
    return measure_randomized<Float>(benchmark, renderer, setup.scene, htracer::rendering::seq);
  }
  return measure_randomized<Float>(benchmark, renderer, setup.scene, htracer::rendering::par);
}

} // namespace


benchmark_result
run_benchmark(benchmark_case const &benchmark)
{
  switch (benchmark.render.precision)
  {
  case precision_kind::f32:
    return run_typed<float>(benchmark);
  case precision_kind::f64:
    return run_typed<double>(benchmark);
  }

  throw std::invalid_argument("unsupported benchmark precision");
}

} // namespace htracer::benchmarks
