#ifndef HTRACER_BENCHMARKS_CLI_INVOCATION_HPP
#define HTRACER_BENCHMARKS_CLI_INVOCATION_HPP


#include <htracer_benchmarks/cli/schema/command_tree.hpp>
#include <htracer_benchmarks/cli_structure/commands/application.hpp>


namespace htracer::benchmarks
{

using invocation = cli_structure::invocation_t<cli::root_command>;

} // namespace htracer::benchmarks

#endif
