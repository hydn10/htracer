#ifndef HTRACER_BENCHMARKS_CLI_PARSERS_REPETITIONS_PARSER_HPP
#define HTRACER_BENCHMARKS_CLI_PARSERS_REPETITIONS_PARSER_HPP


#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>

#include <string_view>


namespace htracer::benchmarks::cli::parsers
{

struct repetitions_parser
{
  [[nodiscard]]
  cli_structure::value_parsing::parse_result<repetition_count>
  operator()(std::string_view text) const;
};

} // namespace htracer::benchmarks::cli::parsers

#endif
