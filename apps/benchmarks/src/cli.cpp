#include <htracer_benchmarks/cli.hpp>

#include <htracer_benchmarks/cli/invocation.hpp>
#include <htracer_benchmarks/cli/schema.hpp>
#include <htracer_benchmarks/cli_structure/application.hpp>

#include <span>


namespace htracer::benchmarks
{
namespace
{

constexpr auto benchmark_cli = cli_structure::make_application(cli::make_root_command());

} // namespace


invocation
parse_cli(std::span<char const *const> arguments)
{
  return benchmark_cli.parse(arguments);
}

} // namespace htracer::benchmarks
