#ifndef HTRACER_BENCHMARKS_CLI_SCHEMA_HPP
#define HTRACER_BENCHMARKS_CLI_SCHEMA_HPP


#include <htracer_benchmarks/cli/commands.hpp>
#include <htracer_benchmarks/cli/parsers.hpp>
#include <htracer_benchmarks/cli/render_configuration.hpp>
#include <htracer_benchmarks/cli_structure/schema/composed_input.hpp>
#include <htracer_benchmarks/cli_structure/schema/command.hpp>
#include <htracer_benchmarks/cli_structure/schema/inputs.hpp>
#include <htracer_benchmarks/cli_structure/schema/leaf_node.hpp>
#include <htracer_benchmarks/cli_structure/schema/object_input.hpp>
#include <htracer_benchmarks/cli_structure/schema/subcommands.hpp>
#include <htracer_benchmarks/cli_structure/errors.hpp>
#include <htracer_benchmarks/cli_structure/schema/options/policies/defaulted_value.hpp>
#include <htracer_benchmarks/cli_structure/schema/options/option_info.hpp>
#include <htracer_benchmarks/cli_structure/schema/options/policies/required_value.hpp>
#include <htracer_benchmarks/cli_structure/schema/options/policies/optional_value.hpp>
#include <htracer_benchmarks/cli_structure/schema/options/kinds/value_option.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>

#include <expected>
#include <string>
#include <string_view>
#include <utility>


namespace htracer::benchmarks::cli
{
namespace schema = cli_structure::schema;
namespace schema_detail
{

template<typename Value>
[[nodiscard]]
constexpr Value
schema_value(std::expected<Value, std::string_view> result);


template<typename Value>
[[nodiscard]]
constexpr Value
schema_value(std::expected<Value, std::string_view> result)
{
  if (!result)
  {
    throw cli_structure::schema_error{std::string{result.error()}};
  }

  return std::move(*result);
}

} // namespace schema_detail


[[nodiscard]]
constexpr auto
make_root_command();


[[nodiscard]]
constexpr auto
make_root_command()
{
  auto const width = schema::required(
      schema::value_option{
          schema::value_option_info{schema::option_name{'w', "width"}, "N", "Image width in pixels"},
          parsers::width_parser{}});

  auto const height = schema::required(
      schema::value_option{
          schema::value_option_info{schema::option_name{'H', "height"}, "N", "Image height in pixels"},
          parsers::height_parser{}});

  auto const precision = schema::required(
      schema::value_option{
          schema::value_option_info{
              schema::option_name{'p', "precision"}, "float|double", "Floating-point precision"},
          parsers::precision_parser{}});

  auto const policy = schema::required(
      schema::value_option{
          schema::value_option_info{schema::option_name{'P', "policy"}, "seq|par", "Execution policy"},
          parsers::policy_parser{}});

  auto const warmups = schema::defaulted(
      schema::value_option{
          schema::value_option_info{
              schema::option_name{'u', "warmups"}, "N", "Warmup render count; zero is allowed"},
          parsers::warmups_parser{}},
      warmup_count{1},
      "1");

  auto const repetitions = schema::defaulted(
      schema::value_option{
          schema::value_option_info{
              schema::option_name{'r', "repetitions"}, "N", "Measured render count"},
          parsers::repetitions_parser{}},
      schema_detail::schema_value(repetition_count::try_make(9)),
      "9");

  auto const output = schema::optional(
      schema::value_option{
          schema::value_option_info{
              schema::option_name{'o', "output"}, "PATH", "Write versioned JSON to this path"},
          parsers::output_parser{}});

  auto const traversal = schema::required(
      schema::value_option{
          schema::value_option_info{
              schema::option_name{'g', "geometry-count"}, "N", "Traversal scene geometry count"},
          parsers::traversal_parser{}});

  auto const samples = schema::required(
      schema::value_option{
          schema::value_option_info{schema::option_name{'s', "samples"}, "N", "Samples per pixel"},
          parsers::samples_parser{}});

  auto const seed = schema::optional(
      schema::value_option{
          schema::value_option_info{
              schema::option_name{'S', "seed"}, "N", "Unsigned random seed; omit for an unseeded run"},
          parsers::seed_parser{}});

  auto const extent = schema::compose<image_extent>(schema::inputs{width, height}, parsers::extent_parser{});

  auto const measurement = schema::object<measurement_plan>(schema::inputs{warmups, repetitions});

  auto const render_options = schema::object<render_configuration>(
      schema::inputs{extent, precision, policy, measurement, output});

  auto const randomized_options = schema::object<randomized_render>(schema::inputs{samples, seed});

  auto const list = schema::command{
      "list", "List canonical benchmark cases.", schema::leaf<list_command>(schema::inputs{})};

  auto const quick = schema::command{
      "quick",
      "Run the canonical quick benchmark suite.",
      schema::leaf<quick_suite_command>(schema::inputs{output})};
  auto const suite =
      schema::command{"suite", "Run a canonical benchmark suite.", schema::subcommands{quick}};

  auto const mixed_deterministic = schema::command{
      "deterministic",
      "Run a deterministic mixed-scene benchmark.",
      schema::leaf<mixed_deterministic_command>(schema::inputs{render_options})};
  auto const mixed_randomized = schema::command{
      "randomized",
      "Run a randomized mixed-scene benchmark.",
      schema::leaf<mixed_randomized_command>(schema::inputs{randomized_options, render_options})};
  auto const mixed = schema::command{
      "mixed",
      "Benchmark the representative mixed scene.",
      schema::subcommands{mixed_deterministic, mixed_randomized}};

  auto const traversal_deterministic = schema::command{
      "deterministic",
      "Run a deterministic traversal benchmark.",
      schema::leaf<traversal_deterministic_command>(schema::inputs{traversal, render_options})};
  auto const traversal_randomized = schema::command{
      "randomized",
      "Run a randomized traversal benchmark.",
      schema::leaf<traversal_randomized_command>(
          schema::inputs{traversal, randomized_options, render_options})};
  auto const traversal_command = schema::command{
      "traversal",
      "Benchmark the geometry traversal diagnostic scene.",
      schema::subcommands{traversal_deterministic, traversal_randomized}};

  auto const rng_probe_randomized = schema::command{
      "randomized",
      "Run the randomized RNG diagnostic benchmark.",
      schema::leaf<rng_probe_randomized_command>(schema::inputs{randomized_options, render_options})};
  auto const rng_probe = schema::command{
      "rng-probe",
      "Benchmark the random-number generation diagnostic scene.",
      schema::subcommands{rng_probe_randomized}};

  auto const render = schema::command{
      "render",
      "Run one explicitly configured render benchmark.",
      schema::subcommands{mixed, traversal_command, rng_probe}};

  return schema::command{
      "htracer-benchmarks",
      "Measure Htracer whole-image rendering performance.",
      schema::subcommands{list, suite, render}};
}

} // namespace htracer::benchmarks::cli

#endif
