#include <htracer_benchmarks/cli_structure/parsing/parsers.hpp>

#include <htracer_benchmarks/cli_structure/foundations/parse_result.hpp>

#include <string>
#include <string_view>


namespace htracer::benchmarks::cli_structure
{

parse_result<std::string>
string_value::operator()(std::string_view text) const
{
  return std::string{text};
}

} // namespace htracer::benchmarks::cli_structure
