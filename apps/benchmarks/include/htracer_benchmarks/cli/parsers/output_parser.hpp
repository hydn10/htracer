#ifndef HTRACER_BENCHMARKS_CLI_PARSERS_OUTPUT_PARSER_HPP
#define HTRACER_BENCHMARKS_CLI_PARSERS_OUTPUT_PARSER_HPP


#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>

#include <filesystem>
#include <string_view>


namespace htracer::benchmarks::cli::parsers
{

struct output_parser
{
  [[nodiscard]]
  cli_structure::value_parsing::parse_result<std::filesystem::path>
  operator()(std::string_view text) const;
};

} // namespace htracer::benchmarks::cli::parsers

#endif
