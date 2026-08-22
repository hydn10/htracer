#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_VALUE_PARSING_PARSERS_STRING_VALUE_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_VALUE_PARSING_PARSERS_STRING_VALUE_HPP


#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>

#include <string>
#include <string_view>


namespace htracer::benchmarks::cli_structure::value_parsing
{

struct string_value
{
  using result_type = std::string;

  [[nodiscard]]
  parse_result<std::string>
  operator()(std::string_view text) const;
};

} // namespace htracer::benchmarks::cli_structure::value_parsing

#endif
