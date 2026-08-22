#include <htracer_benchmarks/app.hpp>

#include <htracer_benchmarks/benchmark_case.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/cli.hpp>
#include <htracer_benchmarks/cli/commands.hpp>
#include <htracer_benchmarks/cli/render_configuration.hpp>
#include <htracer_benchmarks/cli_structure/help_page.hpp>
#include <htracer_benchmarks/quick_suite.hpp>
#include <htracer_benchmarks/render_benchmark.hpp>
#include <htracer_benchmarks/render_mode.hpp>
#include <htracer_benchmarks/reporting/benchmark_name.hpp>
#include <htracer_benchmarks/reporting/console.hpp>
#include <htracer_benchmarks/reporting/environment.hpp>
#include <htracer_benchmarks/reporting/json.hpp>
#include <htracer_benchmarks/run_report.hpp>
#include <htracer_benchmarks/scene_spec.hpp>

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
handle(htracer::benchmarks::cli_structure::help_page const &command)
{
  std::print(std::cout, "{}", command.text);
}


void
handle([[maybe_unused]] htracer::benchmarks::cli::list_command const &command)
{
  htracer::benchmarks::reporting::print_case_list(htracer::benchmarks::quick_suite_catalog::cases());
}


[[nodiscard]]
htracer::benchmarks::benchmark_definition
deterministic_definition(
    htracer::benchmarks::deterministic_scene_spec scene, htracer::benchmarks::cli::render_configuration const &options)
{
  using namespace htracer::benchmarks;
  return benchmark_definition::deterministic(
      scene, options.precision, options.policy, options.extent, options.measurement);
}


[[nodiscard]]
htracer::benchmarks::benchmark_definition
randomized_definition(
    htracer::benchmarks::scene_spec scene,
    htracer::benchmarks::randomized_render const &randomized,
    htracer::benchmarks::cli::render_configuration const &options)
{
  using namespace htracer::benchmarks;
  return benchmark_definition::randomized(
      scene, randomized, options.precision, options.policy, options.extent, options.measurement);
}


void
run_custom(htracer::benchmarks::benchmark_definition definition, std::optional<std::filesystem::path> const &output)
{
  std::array benchmarks{htracer::benchmarks::benchmark_case::custom(definition)};
  run_cases(benchmarks, output);
}


void
handle(htracer::benchmarks::cli::quick_suite_command const &command)
{
  run_cases(htracer::benchmarks::quick_suite_catalog::cases(), command.output);
}


void
handle(htracer::benchmarks::cli::mixed_deterministic_command const &command)
{
  run_custom(deterministic_definition(htracer::benchmarks::mixed_scene{}, command.render), command.render.output);
}


void
handle(htracer::benchmarks::cli::mixed_randomized_command const &command)
{
  run_custom(
      randomized_definition(htracer::benchmarks::mixed_scene{}, command.randomized, command.render),
      command.render.output);
}


void
handle(htracer::benchmarks::cli::traversal_deterministic_command const &command)
{
  run_custom(deterministic_definition(command.traversal, command.render), command.render.output);
}


void
handle(htracer::benchmarks::cli::traversal_randomized_command const &command)
{
  run_custom(randomized_definition(command.traversal, command.randomized, command.render), command.render.output);
}


void
handle(htracer::benchmarks::cli::rng_probe_randomized_command const &command)
{
  run_custom(
      randomized_definition(htracer::benchmarks::rng_probe_scene{}, command.randomized, command.render),
      command.render.output);
}

} // namespace


namespace htracer::benchmarks
{

void
run_app(std::span<char const *const> arguments)
{
  std::visit(
      [](auto &&command)
      {
        return handle(std::forward<decltype(command)>(command));
      },
      parse_cli(arguments));
}

} // namespace htracer::benchmarks
