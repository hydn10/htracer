#include <htracer_benchmarks/app.hpp>

#include <htracer_benchmarks/benchmark_case.hpp>
#include <htracer_benchmarks/cli.hpp>
#include <htracer_benchmarks/invocation.hpp>
#include <htracer_benchmarks/quick_suite.hpp>
#include <htracer_benchmarks/render_benchmark.hpp>
#include <htracer_benchmarks/reporting/benchmark_name.hpp>
#include <htracer_benchmarks/reporting/console.hpp>
#include <htracer_benchmarks/reporting/environment.hpp>
#include <htracer_benchmarks/reporting/json.hpp>
#include <htracer_benchmarks/run_report.hpp>

#include <array>
#include <filesystem>
#include <iostream>
#include <optional>
#include <ostream>
#include <print> // IWYU pragma: keep
#include <span>
#include <utility>
#include <variant>


namespace
{

void
run_cases(
    std::span<htracer::benchmarks::benchmark_case const> cases, std::optional<std::filesystem::path> const &output)
{
  using namespace htracer::benchmarks;
  using namespace htracer::benchmarks::reporting;

  run_report report{.environment = get_environment_info(), .results = {}};
  report.results.reserve(cases.size());
  print_environment(report.environment);

  for (auto const &benchmark : cases)
  {
    std::println(std::cout, "Running {}...", benchmark_name(benchmark));
    std::cout.flush();
    auto result = run_benchmark(benchmark);
    print_result(result);
    report.results.push_back(std::move(result));
  }

  if (output)
  {
    write_json(*output, report);
    std::println(std::cout, "Wrote {}", output->string());
  }
}


void
handle([[maybe_unused]] htracer::benchmarks::help_command const &command)
{
  htracer::benchmarks::print_help();
}


void
handle([[maybe_unused]] htracer::benchmarks::list_command const &command)
{
  htracer::benchmarks::reporting::print_case_list(htracer::benchmarks::quick_suite_catalog::cases());
}


void
handle(htracer::benchmarks::quick_suite_command const &command)
{
  run_cases(htracer::benchmarks::quick_suite_catalog::cases(), command.output);
}


void
handle(htracer::benchmarks::custom_render_command const &command)
{
  std::array benchmarks{htracer::benchmarks::benchmark_case::custom(command.benchmark)};
  run_cases(benchmarks, command.output);
}

} // namespace


namespace htracer::benchmarks
{

void
run_app(std::span<char const *const> arguments)
{
  std::visit([](auto const &command) { return handle(command); }, parse_cli(arguments));
}

} // namespace htracer::benchmarks
