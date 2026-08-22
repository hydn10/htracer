#ifndef HTRACER_BENCHMARKS_CLI_PARSERS_PRECISION_PARSER_HPP
#define HTRACER_BENCHMARKS_CLI_PARSERS_PRECISION_PARSER_HPP


#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>

#include <string_view>


namespace htracer::benchmarks::cli::parsers
{

struct precision_parser
{
  [[nodiscard]]
  cli_structure::value_parsing::parse_result<precision_kind>
  operator()(std::string_view text) const;
};

} // namespace htracer::benchmarks::cli::parsers

#endif
