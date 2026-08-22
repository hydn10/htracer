#include <htracer_benchmarks/cli/parsers/policy_parser.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>

#include <expected>
#include <string_view>

namespace htracer::benchmarks::cli::parsers
{

cli_structure::value_parsing::parse_result<policy_kind>
policy_parser::operator()(std::string_view text) const
{
  if (text == "seq")
  {
    return policy_kind::seq;
  }
  if (text == "par")
  {
    return policy_kind::par;
  }

  return std::unexpected{cli_structure::value_parsing::parse_error{"expected 'seq' or 'par'"}};
}

} // namespace htracer::benchmarks::cli::parsers
