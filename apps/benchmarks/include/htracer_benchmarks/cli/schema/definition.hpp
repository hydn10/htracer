#ifndef HTRACER_BENCHMARKS_CLI_SCHEMA_DEFINITION_HPP
#define HTRACER_BENCHMARKS_CLI_SCHEMA_DEFINITION_HPP


#include <htracer_benchmarks/cli/parsers.hpp>
#include <htracer_benchmarks/cli/schema/command_tree.hpp>
#include <htracer_benchmarks/cli_structure/foundations/errors.hpp>
#include <htracer_benchmarks/cli_structure/options/defaulted_value.hpp>
#include <htracer_benchmarks/cli_structure/options/option_info.hpp>
#include <htracer_benchmarks/cli_structure/options/optional_value.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>

#include <expected>
#include <string>
#include <string_view>
#include <utility>


namespace htracer::benchmarks::cli
{
namespace schema_detail
{

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
constexpr root_command
make_root_command()
{
  auto const width = width_input{
      cli_structure::value_option_info{cli_structure::option_name{'w', "width"}, "N", "Image width in pixels"},
      width_parser{}};

  auto const height = height_input{
      cli_structure::value_option_info{cli_structure::option_name{'H', "height"}, "N", "Image height in pixels"},
      height_parser{}};

  auto const precision = precision_input{
      cli_structure::value_option_info{
          cli_structure::option_name{'p', "precision"}, "float|double", "Floating-point precision"},
      precision_parser{}};

  auto const policy = policy_input{
      cli_structure::value_option_info{cli_structure::option_name{'P', "policy"}, "seq|par", "Execution policy"},
      policy_parser{}};

  auto const warmups = warmups_input{
      cli_structure::value_option_info{
          cli_structure::option_name{'u', "warmups"}, "N", "Warmup render count; zero is allowed"},
      warmups_parser{}};

  auto const repetitions = repetitions_input{
      cli_structure::value_option_info{cli_structure::option_name{'r', "repetitions"}, "N", "Measured render count"},
      repetitions_parser{}};

  auto const output = output_input{
      cli_structure::value_option_info{
          cli_structure::option_name{'o', "output"}, "PATH", "Write versioned JSON to this path"},
      output_parser{}};

  auto const traversal = traversal_input{
      cli_structure::value_option_info{
          cli_structure::option_name{'g', "geometry-count"}, "N", "Traversal scene geometry count"},
      traversal_parser{}};

  auto const samples = samples_input{
      cli_structure::value_option_info{cli_structure::option_name{'s', "samples"}, "N", "Samples per pixel"},
      samples_parser{}};

  auto const seed = seed_input{
      cli_structure::value_option_info{
          cli_structure::option_name{'S', "seed"}, "N", "Unsigned random seed; omit for an unseeded run"},
      seed_parser{}};

  auto const extent = extent_input{extent_inputs{width, height}, extent_parser{}};

  auto const measurement = measurement_plan_input{measurement_inputs{
      cli_structure::defaulted(warmups, warmup_count{1}, "1"),
      cli_structure::defaulted(repetitions, schema_detail::schema_value(repetition_count::try_make(9)), "9")}};

  auto const render = render_configuration_input{
      render_inputs{extent, precision, policy, measurement, cli_structure::optional(output)}};

  auto const randomized = randomized_render_input{randomized_inputs{samples, cli_structure::optional(seed)}};

  return root_command{
      "htracer-benchmarks",
      "Measure Htracer whole-image rendering performance.",
      root_children{
          list_node{"list", "List canonical benchmark cases.", list_leaf{list_inputs{}}},
          suite_node{
              "suite",
              "Run a canonical benchmark suite.",
              suite_children{quick_suite_node{
                  "quick",
                  "Run the canonical quick benchmark suite.",
                  quick_suite_leaf{quick_suite_inputs{cli_structure::optional(output)}}}}},

          render_node{
              "render",
              "Run one explicitly configured render benchmark.",
              render_children{
                  mixed_node{
                      "mixed",
                      "Benchmark the representative mixed scene.",
                      mixed_children{
                          mixed_deterministic_node{
                              "deterministic",
                              "Run a deterministic mixed-scene benchmark.",
                              mixed_deterministic_leaf{mixed_deterministic_inputs{render}}},
                          mixed_randomized_node{
                              "randomized",
                              "Run a randomized mixed-scene benchmark.",
                              mixed_randomized_leaf{mixed_randomized_inputs{randomized, render}}}}},

                  traversal_node{
                      "traversal",
                      "Benchmark the geometry traversal diagnostic scene.",
                      traversal_children{
                          traversal_deterministic_node{
                              "deterministic",
                              "Run a deterministic traversal benchmark.",
                              traversal_deterministic_leaf{traversal_deterministic_inputs{traversal, render}}},
                          traversal_randomized_node{
                              "randomized",
                              "Run a randomized traversal benchmark.",
                              traversal_randomized_leaf{traversal_randomized_inputs{traversal, randomized, render}}}}},

                  rng_probe_node{
                      "rng-probe",
                      "Benchmark the random-number generation diagnostic scene.",
                      rng_probe_children{rng_probe_randomized_node{
                          "randomized",
                          "Run the randomized RNG diagnostic benchmark.",
                          rng_probe_randomized_leaf{rng_probe_randomized_inputs{randomized, render}}}}}}}}};
}

} // namespace htracer::benchmarks::cli

#endif
