#include <htracer_benchmarks/cli/parsers/precision_parser.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>

#include <expected>
#include <string_view>

namespace htracer::benchmarks::cli::parsers
{

cli_structure::value_parsing::parse_result<precision_kind>
precision_parser::operator()(std::string_view text) const
{
  if (text == "float")
  {
    return precision_kind::f32;
  }
  if (text == "double")
  {
    return precision_kind::f64;
  }

  return std::unexpected{cli_structure::value_parsing::parse_error{"expected 'float' or 'double'"}};
}

} // namespace htracer::benchmarks::cli::parsers
