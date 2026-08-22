#include <htracer_benchmarks/cli_structure/value_parsing/parsers/string_value.hpp>

#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>

#include <string>
#include <string_view>


namespace htracer::benchmarks::cli_structure::value_parsing
{

parse_result<std::string>
string_value::operator()(std::string_view text) const
{
  return std::string{text};
}

} // namespace htracer::benchmarks::cli_structure::value_parsing
