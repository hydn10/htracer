#ifndef HTRACER_BENCHMARKS_CLI_HPP
#define HTRACER_BENCHMARKS_CLI_HPP


#include <htracer_benchmarks/cli/invocation.hpp>
#include <htracer_benchmarks/cli_structure/errors.hpp>

#include <span>


namespace htracer::benchmarks
{

using usage_error = cli_structure::usage_error;
using schema_error = cli_structure::schema_error;


[[nodiscard]]
invocation
parse_cli(std::span<char const *const> arguments);

} // namespace htracer::benchmarks

#endif
