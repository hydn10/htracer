#ifndef HTRACER_BENCHMARKS_CLI_CUSTOM_DEFINITION_HPP
#define HTRACER_BENCHMARKS_CLI_CUSTOM_DEFINITION_HPP


#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/cli/raw_options.hpp>


namespace htracer::benchmarks::cli
{

[[nodiscard]]
benchmark_definition
parse_custom_definition(raw_options const &options);

} // namespace htracer::benchmarks::cli

#endif
