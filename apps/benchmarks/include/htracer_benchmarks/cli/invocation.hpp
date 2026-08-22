#ifndef HTRACER_BENCHMARKS_CLI_INVOCATION_HPP
#define HTRACER_BENCHMARKS_CLI_INVOCATION_HPP


#include <htracer_benchmarks/cli/schema.hpp>
#include <htracer_benchmarks/cli_structure/application.hpp>


namespace htracer::benchmarks
{

using invocation = cli_structure::invocation_t<decltype(cli::make_root_command())>;

} // namespace htracer::benchmarks

#endif
