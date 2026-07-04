#ifndef HTRACER_BENCHMARKS_EXECUTION_MEASUREMENT_HPP
#define HTRACER_BENCHMARKS_EXECUTION_MEASUREMENT_HPP


#include <htracer_benchmarks/benchmark_case.hpp>
#include <htracer_benchmarks/benchmark_result.hpp>
#include <htracer_benchmarks/execution/image_validation.hpp>
#include <htracer_benchmarks/image_extent.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>

#include <chrono>
#include <concepts>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>


namespace htracer::benchmarks::execution
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

} // namespace htracer::benchmarks::execution

#endif
