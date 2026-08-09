#include <htracer_benchmarks/benchmark_result.hpp>

#include <htracer_benchmarks/benchmark_case.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/execution/checksum_accumulator.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>
#include <htracer_benchmarks/render_mode.hpp>

#include <algorithm>
#include <chrono>
#include <concepts>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>


namespace htracer::benchmarks
{

namespace
{

[[nodiscard]]
run_checksum
aggregate_checksum(std::vector<measured_render> const &renders) noexcept
{
  execution::checksum_accumulator aggregate;
  for (auto const &render : renders)
  {
    aggregate.append(render.checksum().value());
  }
  return run_checksum{aggregate.value()};
}


[[nodiscard]]
std::optional<run_checksum>
aggregate_checksum(std::vector<image_checksum> const &checksums) noexcept
{
  if (checksums.empty())
  {
    return std::nullopt;
  }

  execution::checksum_accumulator aggregate;
  for (auto const checksum : checksums)
  {
    aggregate.append(checksum.value());
  }
  return run_checksum{aggregate.value()};
}


[[nodiscard]]
duration_summary
summarize(std::vector<measured_render> const &renders)
{
  std::vector<std::chrono::nanoseconds> durations;
  durations.reserve(renders.size());
  for (auto const &render : renders)
  {
    durations.push_back(render.duration());
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


void
require_stable_checksums(
    std::vector<image_checksum> const &warmup_checksums, std::vector<measured_render> const &renders)
{
  std::optional<image_checksum> expected;
  auto const require_equal = [&expected](image_checksum checksum)
  {
    if (expected && *expected != checksum)
    {
      throw std::invalid_argument("deterministic or seeded render checksums must agree");
    }
    expected = checksum;
  };

  for (auto const checksum : warmup_checksums)
  {
    require_equal(checksum);
  }
  for (auto const &render : renders)
  {
    require_equal(render.checksum());
  }
}


void
validate_output_consistency(
    render_mode const &rendering,
    std::vector<image_checksum> const &warmup_checksums,
    std::vector<measured_render> const &renders)
{
  std::visit(
      [&]<typename Mode>(Mode const &mode)
  {
    if constexpr (std::same_as<Mode, deterministic_render>)
    {
      require_stable_checksums(warmup_checksums, renders);
    }
    else
    {
      static_assert(std::same_as<Mode, randomized_render>);
      if (mode.seed())
      {
        require_stable_checksums(warmup_checksums, renders);
      }
    }
  },
      rendering);
}

} // namespace


measured_render
measured_render::make(std::chrono::nanoseconds duration, image_checksum checksum)
{
  if (duration.count() <= 0)
  {
    throw std::invalid_argument("measured render duration must be greater than zero");
  }
  return measured_render{duration, checksum};
}


benchmark_result::benchmark_result(
    benchmark_case benchmark,
    std::vector<measured_render> renders,
    duration_summary summary,
    run_checksum checksum,
    std::vector<image_checksum> warmup_checksums,
    std::optional<run_checksum> warmup_checksum)
    : benchmark_{benchmark}
    , renders_{std::move(renders)}
    , summary_{summary}
    , checksum_{checksum}
    , warmup_checksums_{std::move(warmup_checksums)}
    , warmup_checksum_{warmup_checksum}
{
}


benchmark_result
benchmark_result::make(
    benchmark_case benchmark, std::vector<measured_render> renders, std::vector<image_checksum> warmup_checksums)
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
  if (warmup_checksums.size() != measurement.warmups.value())
  {
    throw std::invalid_argument("warmup checksum count does not match the benchmark warmup count");
  }

  validate_output_consistency(benchmark.definition().rendering(), warmup_checksums, renders);
  auto const summary = summarize(renders);
  auto const checksum = aggregate_checksum(renders);
  auto const warmup_checksum = aggregate_checksum(warmup_checksums);
  return benchmark_result{
      benchmark, std::move(renders), summary, checksum, std::move(warmup_checksums), warmup_checksum};
}

} // namespace htracer::benchmarks
