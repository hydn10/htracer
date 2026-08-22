#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_VALUE_PARSING_PARSE_RESULT_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_VALUE_PARSING_PARSE_RESULT_HPP


#include <expected>
#include <string>


namespace htracer::benchmarks::cli_structure::value_parsing
{

struct parse_error
{
  std::string message;
};

template<typename Result>
using parse_result = std::expected<Result, parse_error>;

} // namespace htracer::benchmarks::cli_structure::value_parsing

#endif
