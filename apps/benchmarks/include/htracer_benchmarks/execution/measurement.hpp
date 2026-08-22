#ifndef HTRACER_BENCHMARKS_EXECUTION_MEASUREMENT_HPP
#define HTRACER_BENCHMARKS_EXECUTION_MEASUREMENT_HPP


#include <htracer/float_traits.hpp>
#include <htracer_benchmarks/benchmark_case.hpp>
#include <htracer_benchmarks/benchmark_result.hpp>
#include <htracer_benchmarks/execution/image_validation.hpp>

#include <chrono>
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>


namespace htracer::benchmarks::execution
{

template<std::floating_point Float, typename RenderOnce>
requires std::regular_invocable<RenderOnce const &> &&
         std::same_as<std::invoke_result_t<RenderOnce const &>, typename htracer::float_traits<Float>::image>
[[nodiscard]]
benchmark_result
measure(benchmark_case const &benchmark, RenderOnce const &render_once);


template<std::floating_point Float, typename RenderOnce>
requires std::regular_invocable<RenderOnce const &> &&
         std::same_as<std::invoke_result_t<RenderOnce const &>, typename htracer::float_traits<Float>::image>
[[nodiscard]]
benchmark_result
measure(benchmark_case const &benchmark, RenderOnce const &render_once)
{
  auto const extent = benchmark.definition().extent();
  auto const plan = benchmark.definition().measurement();

  std::vector<image_checksum> warmup_checksums;
  warmup_checksums.reserve(plan.warmups.value());
  for (std::uint32_t warmup = 0; warmup < plan.warmups.value(); ++warmup)
  {
    warmup_checksums.push_back(validate_and_hash_image<Float>(render_once(), extent));
  }

  std::vector<measured_render> renders;
  renders.reserve(plan.repetitions.value());

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
    renders.push_back(measured_render::make(duration, checksum));
  }

  return benchmark_result::make(benchmark, std::move(renders), std::move(warmup_checksums));
}

} // namespace htracer::benchmarks::execution

#endif
