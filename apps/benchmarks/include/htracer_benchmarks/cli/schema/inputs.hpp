#ifndef HTRACER_BENCHMARKS_CLI_SCHEMA_INPUTS_HPP
#define HTRACER_BENCHMARKS_CLI_SCHEMA_INPUTS_HPP


#include <htracer_benchmarks/cli/commands.hpp>
#include <htracer_benchmarks/cli/parsers.hpp>
#include <htracer_benchmarks/cli_structure/composition/composed_input.hpp>
#include <htracer_benchmarks/cli_structure/composition/inputs.hpp>
#include <htracer_benchmarks/cli_structure/composition/object_input.hpp>
#include <htracer_benchmarks/cli_structure/options/defaulted_value.hpp>
#include <htracer_benchmarks/cli_structure/options/optional_value.hpp>
#include <htracer_benchmarks/cli_structure/options/value_option.hpp>


namespace htracer::benchmarks::cli
{

using width_input = cli_structure::value_option<width_option, width_parser>;
using height_input = cli_structure::value_option<height_option, height_parser>;
using precision_input = cli_structure::value_option<precision_option, precision_parser>;
using policy_input = cli_structure::value_option<policy_option, policy_parser>;
using warmups_input = cli_structure::value_option<warmups_option, warmups_parser>;
using repetitions_input = cli_structure::value_option<repetitions_option, repetitions_parser>;
using output_input = cli_structure::value_option<output_option, output_parser>;
using traversal_input = cli_structure::value_option<traversal_option, traversal_parser>;
using samples_input = cli_structure::value_option<samples_option, samples_parser>;
using seed_input = cli_structure::value_option<seed_option, seed_parser>;

using extent_inputs = cli_structure::inputs<width_input, height_input>;
using extent_input = cli_structure::composed_input<extent_option, extent_inputs, extent_parser>;

using render_inputs = cli_structure::inputs<
    extent_input,
    precision_input,
    policy_input,
    cli_structure::defaulted_value<warmups_input>,
    cli_structure::defaulted_value<repetitions_input>,
    cli_structure::optional_value<output_input>>;
using render_options_input = cli_structure::object_input<render_options, render_inputs>;

using randomized_inputs = cli_structure::inputs<samples_input, cli_structure::optional_value<seed_input>>;
using randomized_options_input = cli_structure::object_input<randomized_options, randomized_inputs>;

using list_inputs = cli_structure::inputs<>;
using quick_suite_inputs = cli_structure::inputs<cli_structure::optional_value<output_input>>;
using mixed_deterministic_inputs = cli_structure::inputs<render_options_input>;
using mixed_randomized_inputs = cli_structure::inputs<randomized_options_input, render_options_input>;
using traversal_deterministic_inputs = cli_structure::inputs<traversal_input, render_options_input>;
using traversal_randomized_inputs =
    cli_structure::inputs<traversal_input, randomized_options_input, render_options_input>;
using rng_probe_randomized_inputs = cli_structure::inputs<randomized_options_input, render_options_input>;

} // namespace htracer::benchmarks::cli

#endif
