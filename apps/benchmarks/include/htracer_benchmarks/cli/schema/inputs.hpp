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

#include <filesystem>


namespace htracer::benchmarks::cli
{

using width_input = cli_structure::value_option<image_width, width_parser>;
using height_input = cli_structure::value_option<image_height, height_parser>;
using precision_input = cli_structure::value_option<precision_kind, precision_parser>;
using policy_input = cli_structure::value_option<policy_kind, policy_parser>;
using warmups_input = cli_structure::value_option<warmup_count, warmups_parser>;
using repetitions_input = cli_structure::value_option<repetition_count, repetitions_parser>;
using output_input = cli_structure::value_option<std::filesystem::path, output_parser>;
using traversal_input = cli_structure::value_option<traversal_scene, traversal_parser>;
using samples_input = cli_structure::value_option<htracer::rendering::samples_per_pixel, samples_parser>;
using seed_input = cli_structure::value_option<htracer::rendering::random_seed, seed_parser>;

using extent_inputs = cli_structure::inputs<width_input, height_input>;
using extent_input = cli_structure::composed_input<image_extent, extent_inputs, extent_parser>;

using measurement_inputs = cli_structure::
    inputs<cli_structure::defaulted_value<warmups_input>, cli_structure::defaulted_value<repetitions_input>>;
using measurement_plan_input = cli_structure::object_input<measurement_plan, measurement_inputs>;

using render_inputs = cli_structure::inputs<
    extent_input,
    precision_input,
    policy_input,
    measurement_plan_input,
    cli_structure::optional_value<output_input>>;
using render_configuration_input = cli_structure::object_input<render_configuration, render_inputs>;

using randomized_inputs = cli_structure::inputs<samples_input, cli_structure::optional_value<seed_input>>;
using randomized_render_input = cli_structure::object_input<randomized_render, randomized_inputs>;

using list_inputs = cli_structure::inputs<>;
using quick_suite_inputs = cli_structure::inputs<cli_structure::optional_value<output_input>>;
using mixed_deterministic_inputs = cli_structure::inputs<render_configuration_input>;
using mixed_randomized_inputs = cli_structure::inputs<randomized_render_input, render_configuration_input>;
using traversal_deterministic_inputs = cli_structure::inputs<traversal_input, render_configuration_input>;
using traversal_randomized_inputs =
    cli_structure::inputs<traversal_input, randomized_render_input, render_configuration_input>;
using rng_probe_randomized_inputs = cli_structure::inputs<randomized_render_input, render_configuration_input>;

} // namespace htracer::benchmarks::cli

#endif
