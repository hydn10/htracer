#include <htracer_benchmarks/model.hpp>

#include <htracer/rendering/random_seed.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
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


[[nodiscard]]
image_checksum
aggregate_checksum(std::vector<measured_render> const &renders) noexcept
{
  std::uint64_t aggregate = fnv_offset_basis;
  for (auto const &render : renders)
  {
    auto value = render.checksum.value();
    for (std::size_t index = 0; index < sizeof(value); ++index)
    {
      aggregate ^= static_cast<std::uint8_t>(value & 0xffU);
      aggregate *= fnv_prime;
      value >>= 8U;
    }
  }
  return image_checksum{aggregate};
}


[[nodiscard]]
duration_summary
summarize(std::vector<measured_render> const &renders)
{
  std::vector<std::chrono::nanoseconds> durations;
  durations.reserve(renders.size());
  for (auto const &render : renders)
  {
    durations.push_back(render.duration);
  }
  std::ranges::sort(durations);

  auto const middle = durations.size() / 2;
  auto median = durations[middle];
  if (durations.size() % 2 == 0)
  {
    auto const lower = durations[middle - 1];
    auto const upper = durations[middle];
    median = lower + (upper - lower) / 2;
  }
  return {.minimum = durations.front(), .median = median, .maximum = durations.back()};
}

} // namespace

image_extent
image_extent::make(std::uint32_t width, std::uint32_t height)
{
  if (width == 0 || height == 0)
  {
    throw std::invalid_argument("image dimensions must be greater than zero");
  }
  if (width > std::numeric_limits<std::uint32_t>::max() / height)
  {
    throw std::invalid_argument("image pixel count exceeds htracer's supported range");
  }

  constexpr std::size_t largest_pixel_size = 3 * sizeof(double);
  auto const pixels = static_cast<std::size_t>(width) * height;
  if (pixels > std::numeric_limits<std::size_t>::max() / largest_pixel_size)
  {
    throw std::invalid_argument("image storage size exceeds the addressable range");
  }
  return image_extent{width, height};
}


geometry_count
geometry_count::make(std::uint32_t value)
{
  if (value == 0)
  {
    throw std::invalid_argument("geometry count must be greater than zero");
  }
  return geometry_count{value};
}


repetition_count
repetition_count::make(std::uint32_t value)
{
  if (value == 0)
  {
    throw std::invalid_argument("repetition count must be greater than zero");
  }
  return repetition_count{value};
}


randomized_render
randomized_render::make(
    htracer::rendering::samples_per_pixel samples, std::optional<htracer::rendering::random_seed> seed)
{
  if (samples.value == 0)
  {
    throw std::invalid_argument("samples per pixel must be greater than zero");
  }
  return randomized_render{samples, seed};
}


benchmark_definition::benchmark_definition(
    scene_spec scene,
    render_mode rendering,
    precision_kind precision,
    policy_kind policy,
    image_extent extent,
    measurement_plan measurement)
    : scene_{scene}
    , rendering_{rendering}
    , precision_{precision}
    , policy_{policy}
    , extent_{extent}
    , measurement_{measurement}
{
}


benchmark_definition
benchmark_definition::deterministic(
    scene_spec scene, precision_kind precision, policy_kind policy, image_extent extent, measurement_plan measurement)
{
  if (std::holds_alternative<rng_probe_scene>(scene))
  {
    throw std::invalid_argument("rng-probe scene requires randomized rendering");
  }
  return {scene, deterministic_render{}, precision, policy, extent, measurement};
}


benchmark_definition
benchmark_definition::randomized(
    scene_spec scene,
    randomized_render rendering,
    precision_kind precision,
    policy_kind policy,
    image_extent extent,
    measurement_plan measurement)
{
  return {scene, rendering, precision, policy, extent, measurement};
}


benchmark_case::benchmark_case(origin provenance, benchmark_definition definition)
    : origin_{provenance}
    , definition_{definition}
{
}


benchmark_case
benchmark_case::canonical(benchmark_definition definition)
{
  return {origin::canonical, definition};
}


benchmark_case
benchmark_case::custom(benchmark_definition definition)
{
  return {origin::custom, definition};
}


benchmark_result::benchmark_result(
    benchmark_case benchmark,
    std::vector<measured_render> renders,
    duration_summary summary,
    image_checksum checksum,
    std::optional<image_checksum> warmup_checksum)
    : benchmark_{benchmark}
    , renders_{std::move(renders)}
    , summary_{summary}
    , checksum_{checksum}
    , warmup_checksum_{warmup_checksum}
{
}


benchmark_result
benchmark_result::make(
    benchmark_case benchmark, std::vector<measured_render> renders, std::optional<image_checksum> warmup_checksum)
{
  auto const measurement = benchmark.definition().measurement();
  if (renders.size() != measurement.repetitions.value())
  {
    throw std::invalid_argument("measured render count does not match the benchmark repetition count");
  }
  if (renders.empty())
  {
    throw std::invalid_argument("a benchmark result requires at least one measured render");
  }
  for (auto const &render : renders)
  {
    if (render.duration.count() <= 0)
    {
      throw std::invalid_argument("measured render duration must be greater than zero");
    }
  }
  auto const expects_warmup_checksum = measurement.warmups.value() != 0;
  if (warmup_checksum.has_value() != expects_warmup_checksum)
  {
    throw std::invalid_argument("warmup checksum presence does not match the benchmark warmup count");
  }

  auto const summary = summarize(renders);
  auto const checksum = aggregate_checksum(renders);
  return benchmark_result{benchmark, std::move(renders), summary, checksum, warmup_checksum};
}

} // namespace htracer::benchmarks
