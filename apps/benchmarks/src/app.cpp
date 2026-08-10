#include <htracer_benchmarks/app.hpp>

#include <htracer_benchmarks/benchmark_case.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/cli.hpp>
#include <htracer_benchmarks/cli/commands.hpp>
#include <htracer_benchmarks/cli/option_values.hpp>
#include <htracer_benchmarks/cli_structure/foundations/help_page.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>
#include <htracer_benchmarks/quick_suite.hpp>
#include <htracer_benchmarks/render_benchmark.hpp>
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
handle([[maybe_unused]] htracer::benchmarks::list_command const &command)
{
  htracer::benchmarks::reporting::print_case_list(htracer::benchmarks::quick_suite_catalog::cases());
}


[[nodiscard]]
std::optional<std::filesystem::path>
output_path(std::optional<htracer::benchmarks::output_option> const &output)
{
  return output.transform(
      [](htracer::benchmarks::output_option const &option)
      {
        return option.value;
      });
}


[[nodiscard]]
htracer::benchmarks::benchmark_definition
deterministic_definition(
    htracer::benchmarks::deterministic_scene_spec scene, htracer::benchmarks::render_options const &options)
{
  using namespace htracer::benchmarks;
  return benchmark_definition::deterministic(
      scene,
      options.precision.value,
      options.policy.value,
      options.extent.value,
      measurement_plan{.warmups = options.warmups.value, .repetitions = options.repetitions.value});
}


[[nodiscard]]
htracer::benchmarks::benchmark_definition
randomized_definition(
    htracer::benchmarks::scene_spec scene,
    htracer::benchmarks::randomized_options const &randomized,
    htracer::benchmarks::render_options const &options)
{
  using namespace htracer::benchmarks;
  return benchmark_definition::randomized(
      scene,
      randomized_render{
          randomized.samples.value,
          randomized.seed.transform(
              [](seed_option const &option)
              {
                return option.value;
              })},
      options.precision.value,
      options.policy.value,
      options.extent.value,
      measurement_plan{.warmups = options.warmups.value, .repetitions = options.repetitions.value});
}


void
run_custom(
    htracer::benchmarks::benchmark_definition definition,
    std::optional<htracer::benchmarks::output_option> const &output)
{
  std::array benchmarks{htracer::benchmarks::benchmark_case::custom(definition)};
  run_cases(benchmarks, output_path(output));
}


void
handle(htracer::benchmarks::quick_suite_command const &command)
{
  run_cases(htracer::benchmarks::quick_suite_catalog::cases(), output_path(command.output));
}


void
handle(htracer::benchmarks::mixed_deterministic_command const &command)
{
  run_custom(deterministic_definition(htracer::benchmarks::mixed_scene{}, command.render), command.render.output);
}


void
handle(htracer::benchmarks::mixed_randomized_command const &command)
{
  run_custom(
      randomized_definition(htracer::benchmarks::mixed_scene{}, command.randomized, command.render),
      command.render.output);
}


void
handle(htracer::benchmarks::traversal_deterministic_command const &command)
{
  run_custom(deterministic_definition(command.traversal.value, command.render), command.render.output);
}


void
handle(htracer::benchmarks::traversal_randomized_command const &command)
{
  run_custom(randomized_definition(command.traversal.value, command.randomized, command.render), command.render.output);
}


void
handle(htracer::benchmarks::rng_probe_randomized_command const &command)
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
      [](auto const &command)
      {
        return handle(command);
      },
      parse_cli(arguments));
}

} // namespace htracer::benchmarks
