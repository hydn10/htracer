#include <htracer_benchmarks/cli_structure/foundations/errors.hpp>

#include <stdexcept>
#include <string>
#include <utility>


namespace htracer::benchmarks::cli_structure
{

usage_error::usage_error(std::string const &message, std::string help_path)
    : std::runtime_error{message}
    , help_path_{std::move(help_path)}
{
}


std::string const &
usage_error::help_path() const noexcept
{
  return help_path_;
}

} // namespace htracer::benchmarks::cli_structure
