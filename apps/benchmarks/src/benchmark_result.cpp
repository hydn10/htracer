#include <htracer_benchmarks/benchmark_result.hpp>

#include <htracer_benchmarks/benchmark_case.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <utility>
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
