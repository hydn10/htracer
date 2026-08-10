#ifndef HTRACER_BENCHMARKS_CLI_SCHEMA_COMMAND_TREE_HPP
#define HTRACER_BENCHMARKS_CLI_SCHEMA_COMMAND_TREE_HPP


#include <htracer_benchmarks/cli/schema/inputs.hpp>
#include <htracer_benchmarks/cli_structure/commands/command.hpp>
#include <htracer_benchmarks/cli_structure/commands/leaf_node.hpp>
#include <htracer_benchmarks/cli_structure/commands/subcommands.hpp>


namespace htracer::benchmarks::cli
{

using list_leaf = cli_structure::leaf_node<list_command, list_inputs>;
using list_node = cli_structure::command<list_leaf>;

using quick_suite_leaf = cli_structure::leaf_node<quick_suite_command, quick_suite_inputs>;
using quick_suite_node = cli_structure::command<quick_suite_leaf>;
using suite_children = cli_structure::subcommands<quick_suite_node>;
using suite_node = cli_structure::command<suite_children>;

using mixed_deterministic_leaf = cli_structure::leaf_node<mixed_deterministic_command, mixed_deterministic_inputs>;
using mixed_deterministic_node = cli_structure::command<mixed_deterministic_leaf>;

using mixed_randomized_leaf = cli_structure::leaf_node<mixed_randomized_command, mixed_randomized_inputs>;
using mixed_randomized_node = cli_structure::command<mixed_randomized_leaf>;

using mixed_children = cli_structure::subcommands<mixed_deterministic_node, mixed_randomized_node>;
using mixed_node = cli_structure::command<mixed_children>;

using traversal_deterministic_leaf =
    cli_structure::leaf_node<traversal_deterministic_command, traversal_deterministic_inputs>;
using traversal_deterministic_node = cli_structure::command<traversal_deterministic_leaf>;

using traversal_randomized_leaf = cli_structure::leaf_node<traversal_randomized_command, traversal_randomized_inputs>;
using traversal_randomized_node = cli_structure::command<traversal_randomized_leaf>;

using traversal_children = cli_structure::subcommands<traversal_deterministic_node, traversal_randomized_node>;
using traversal_node = cli_structure::command<traversal_children>;

using rng_probe_randomized_leaf = cli_structure::leaf_node<rng_probe_randomized_command, rng_probe_randomized_inputs>;
using rng_probe_randomized_node = cli_structure::command<rng_probe_randomized_leaf>;
using rng_probe_children = cli_structure::subcommands<rng_probe_randomized_node>;
using rng_probe_node = cli_structure::command<rng_probe_children>;

using render_children = cli_structure::subcommands<mixed_node, traversal_node, rng_probe_node>;
using render_node = cli_structure::command<render_children>;

using root_children = cli_structure::subcommands<list_node, suite_node, render_node>;
using root_command = cli_structure::command<root_children>;

} // namespace htracer::benchmarks::cli

#endif
