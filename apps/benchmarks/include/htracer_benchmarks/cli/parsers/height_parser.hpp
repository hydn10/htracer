#ifndef HTRACER_BENCHMARKS_CLI_PARSERS_HEIGHT_PARSER_HPP
#define HTRACER_BENCHMARKS_CLI_PARSERS_HEIGHT_PARSER_HPP


#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>
#include <htracer_benchmarks/image_extent.hpp>

#include <string_view>


namespace htracer::benchmarks::cli::parsers
{

struct height_parser
{
  [[nodiscard]]
  cli_structure::value_parsing::parse_result<image_height>
  operator()(std::string_view text) const;
};

} // namespace htracer::benchmarks::cli::parsers

#endif
