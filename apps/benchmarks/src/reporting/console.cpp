#include <htracer_benchmarks/reporting/console.hpp>

#include <htracer_benchmarks/benchmark_case.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/benchmark_result.hpp>
#include <htracer_benchmarks/render_mode.hpp>
#include <htracer_benchmarks/reporting/benchmark_name.hpp>
#include <htracer_benchmarks/reporting/result_values.hpp>
#include <htracer_benchmarks/run_report.hpp>

#include <chrono>
#include <concepts>
#include <print>
#include <span>
#include <variant>


namespace htracer::benchmarks::reporting
{

void
print_case_list(std::span<benchmark_case const> benchmarks)
{
  std::println("quick suite ({} cases):", benchmarks.size());
  for (auto const &benchmark : benchmarks)
  {
    auto const &definition = benchmark.definition();
    std::print(
        "  {} [{}x{}, {}, {}, {}, {}",
        benchmark_name(benchmark),
        definition.extent().width(),
        definition.extent().height(),
        scene_name(definition.scene()),
        rendering_name(definition.rendering()),
        precision_name(definition.precision()),
        policy_name(definition.policy()));
    std::visit(
        []<typename Mode>(Mode const &mode)
    {
      if constexpr (std::same_as<Mode, randomized_render>)
      {
        std::print(", spp={}", mode.samples().value);
      }
    },
        definition.rendering());
    std::println("]");
  }
}


void
print_environment(environment_info const &environment)
{
  std::println(
      "htracer benchmarks\n"
      "  OS: {} {}\n"
      "  Compiler: {} {}\n"
      "  Build: {}\n"
      "  Logical processors: {}\n",
      environment.os,
      environment.architecture,
      environment.compiler,
      environment.compiler_version,
      environment.build_type,
      environment.logical_processors);
}


void
print_result(benchmark_result const &result)
{
  auto const summary = result.summary();
  auto const ns_to_ms = [](std::chrono::nanoseconds duration)
  {
    return static_cast<long double>(duration.count()) / 1'000'000.0L;
  };
  std::println(
      "{}\n"
      "  median/min/max: {:.3f} / {:.3f} / {:.3f} ms\n"
      "  throughput: {:.3f} Mpixel/s, {:.3f} Mprimary-sample/s\n"
      "  checksum: {}\n",
      benchmark_name(result.benchmark()),
      ns_to_ms(summary.median),
      ns_to_ms(summary.minimum),
      ns_to_ms(summary.maximum),
      pixels_per_second(result) / 1'000'000.0L,
      primary_samples_per_second(result) / 1'000'000.0L,
      hex_checksum(result.checksum()));
}

} // namespace htracer::benchmarks::reporting
