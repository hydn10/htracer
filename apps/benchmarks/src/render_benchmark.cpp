#include <htracer_benchmarks/render_benchmark.hpp>

#include <htracer/htracer.hpp>
#include <htracer_benchmarks/model.hpp>
#include <htracer_benchmarks/scenes.hpp>

#include <chrono>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <stdexcept>
#include <utility>
#include <variant>
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
image_checksum
validate_and_hash_image(typename htracer::float_traits<Float>::image const &image, image_extent extent)
{
  if (image.h_res() != extent.width() || image.v_res() != extent.height())
  {
    throw std::runtime_error("render returned unexpected image dimensions");
  }
  if (image.pixels().size() != extent.pixel_count())
  {
    throw std::runtime_error("render returned unexpected pixel count");
  }

  std::uint64_t hash = fnv_offset_basis;
  auto const width = extent.width();
  auto const height = extent.height();
  hash_value(hash, width);
  hash_value(hash, height);

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
  return image_checksum{hash};
}


void
combine_hash(std::uint64_t &aggregate, image_checksum checksum) noexcept
{
  auto const value = checksum.value();
  hash_value(aggregate, value);
}


} // namespace


namespace
{

template<std::floating_point Float, typename RenderOnce>
[[nodiscard]]
benchmark_result
measure(
    benchmark_case const &benchmark,
    image_extent extent,
    measurement_plan plan,
    bool reproducible,
    RenderOnce const &render_once)
{
  std::uint64_t warmup_hash = fnv_offset_basis;
  for (std::uint32_t warmup = 0; warmup < plan.warmups.value(); ++warmup)
  {
    combine_hash(warmup_hash, validate_and_hash_image<Float>(render_once(), extent));
  }
  auto const warmup_checksum = plan.warmups.value() == 0 ? std::nullopt : std::optional{image_checksum{warmup_hash}};

  std::vector<measured_render> renders;
  renders.reserve(plan.repetitions.value());
  std::optional<image_checksum> expected_image_checksum;

  for (std::uint32_t repetition = 0; repetition < plan.repetitions.value(); ++repetition)
  {
    auto const started_at = std::chrono::steady_clock::now();
    auto const image = render_once();
    auto const stopped_at = std::chrono::steady_clock::now();
    auto const duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stopped_at - started_at);
    if (duration.count() <= 0)
    {
      throw std::runtime_error("measured duration is zero; use a larger workload");
    }

    auto const checksum = validate_and_hash_image<Float>(image, extent);
    if (reproducible)
    {
      if (expected_image_checksum && *expected_image_checksum != checksum)
      {
        throw std::runtime_error("deterministic or seeded render changed across repetitions");
      }
      expected_image_checksum = checksum;
    }
    renders.push_back({.duration = duration, .checksum = checksum});
  }

  return benchmark_result::make(benchmark, std::move(renders), warmup_checksum);
}


template<std::floating_point Float, typename Renderer, typename Scene, htracer::rendering::rendering_policy Policy>
[[nodiscard]]
benchmark_result
measure_deterministic(
    benchmark_case const &benchmark,
    image_extent extent,
    measurement_plan plan,
    Renderer const &renderer,
    Scene const &scene,
    Policy policy)
{
  return measure<Float>(
      benchmark, extent, plan, true, [&renderer, &scene, policy]() { return renderer.render(policy, scene); });
}


template<std::floating_point Float, typename Renderer, typename Scene, htracer::rendering::rendering_policy Policy>
[[nodiscard]]
benchmark_result
measure_randomized(
    benchmark_case const &benchmark,
    image_extent extent,
    measurement_plan plan,
    randomized_render rendering,
    Renderer const &renderer,
    Scene const &scene,
    Policy policy)
{
  if (rendering.seed())
  {
    return measure<Float>(
        benchmark,
        extent,
        plan,
        true,
        [&renderer, &scene, policy, rendering]()
    { return renderer.render(policy, scene, rendering.samples(), *rendering.seed()); });
  }
  return measure<Float>(
      benchmark,
      extent,
      plan,
      false,
      [&renderer, &scene, policy, rendering]() { return renderer.render(policy, scene, rendering.samples()); });
}


template<std::floating_point Float, typename Renderer, typename Scene, typename Mode>
[[nodiscard]]
benchmark_result
dispatch_policy(
    benchmark_case const &benchmark,
    policy_kind execution_policy,
    image_extent extent,
    measurement_plan plan,
    Mode mode,
    Renderer const &renderer,
    Scene const &scene)
{
  switch (execution_policy)
  {
  case policy_kind::seq:
    if constexpr (std::same_as<Mode, deterministic_render>)
    {
      return measure_deterministic<Float>(benchmark, extent, plan, renderer, scene, htracer::rendering::seq);
    }
    else
    {
      return measure_randomized<Float>(benchmark, extent, plan, mode, renderer, scene, htracer::rendering::seq);
    }
  case policy_kind::par:
    if constexpr (std::same_as<Mode, deterministic_render>)
    {
      return measure_deterministic<Float>(benchmark, extent, plan, renderer, scene, htracer::rendering::par);
    }
    else
    {
      return measure_randomized<Float>(benchmark, extent, plan, mode, renderer, scene, htracer::rendering::par);
    }
  }
  std::unreachable();
}


template<std::floating_point Float>
[[nodiscard]]
benchmark_result
run_typed(benchmark_case const &benchmark, benchmark_definition const &definition)
{
  using traits = htracer::float_traits<Float>;

  auto setup = make_scene<Float>(definition.scene());
  auto const extent = definition.extent();
  typename traits::camera const camera{
      setup.camera_position, setup.camera_view, setup.camera_up, extent.width(), extent.height(), setup.fov};
  htracer::rendering::batchers::column_batcher const batcher;
  typename traits::pinhole_lens const lens;

  return std::visit(
      [&]<typename Mode>(Mode mode) -> benchmark_result
  {
    if constexpr (std::same_as<Mode, deterministic_render>)
    {
      typename traits::point_sensor const sensor;
      auto const renderer = htracer::rendering::make_renderer(camera, batcher, sensor, lens);
      return dispatch_policy<Float>(
          benchmark, definition.policy(), definition.extent(), definition.measurement(), mode, renderer, setup.scene);
    }
    else
    {
      static_assert(std::same_as<Mode, randomized_render>);
      typename traits::uniform_sensor const sensor;
      auto const renderer = htracer::rendering::make_renderer(camera, batcher, sensor, lens);
      return dispatch_policy<Float>(
          benchmark, definition.policy(), definition.extent(), definition.measurement(), mode, renderer, setup.scene);
    }
  },
      definition.rendering());
}


[[nodiscard]]
benchmark_result
run_definition(benchmark_case const &benchmark, benchmark_definition const &definition)
{
  switch (definition.precision())
  {
  case precision_kind::f32:
    return run_typed<float>(benchmark, definition);
  case precision_kind::f64:
    return run_typed<double>(benchmark, definition);
  }
  std::unreachable();
}

} // namespace


benchmark_result
run_benchmark(benchmark_case const &benchmark)
{
  return run_definition(benchmark, benchmark.definition());
}

} // namespace htracer::benchmarks
