#include <htracer_benchmarks/cli.hpp>

#include <htracer_benchmarks/cli/invocation.hpp>
#include <htracer_benchmarks/cli/schema.hpp>
#include <htracer_benchmarks/cli_structure/commands/application.hpp>

#include <span>


namespace htracer::benchmarks
{
namespace
{

[[nodiscard]]
auto const &
benchmark_cli()
{
  static auto const result = cli_structure::make_application(cli::make_root_command());
  return result;
}

} // namespace


invocation
parse_cli(std::span<char const *const> arguments)
{
  return benchmark_cli().parse(arguments);
}

} // namespace htracer::benchmarks
