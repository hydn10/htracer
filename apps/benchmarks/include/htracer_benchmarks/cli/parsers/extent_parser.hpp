#ifndef HTRACER_BENCHMARKS_CLI_PARSERS_EXTENT_PARSER_HPP
#define HTRACER_BENCHMARKS_CLI_PARSERS_EXTENT_PARSER_HPP


#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>
#include <htracer_benchmarks/image_extent.hpp>


namespace htracer::benchmarks::cli::parsers
{

struct extent_parser
{
  [[nodiscard]]
  cli_structure::value_parsing::parse_result<image_extent>
  operator()(image_width width, image_height height) const;
};

} // namespace htracer::benchmarks::cli::parsers

#endif
