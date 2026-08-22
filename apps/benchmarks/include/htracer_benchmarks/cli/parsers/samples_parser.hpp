#ifndef HTRACER_BENCHMARKS_CLI_PARSERS_SAMPLES_PARSER_HPP
#define HTRACER_BENCHMARKS_CLI_PARSERS_SAMPLES_PARSER_HPP


#include <htracer/rendering/samples_per_pixel.hpp>
#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>

#include <string_view>


namespace htracer::benchmarks::cli::parsers
{

struct samples_parser
{
  [[nodiscard]]
  cli_structure::value_parsing::parse_result<htracer::rendering::samples_per_pixel>
  operator()(std::string_view text) const;
};

} // namespace htracer::benchmarks::cli::parsers

#endif
