#ifndef HTRACER_BENCHMARKS_CLI_PARSERS_TRAVERSAL_PARSER_HPP
#define HTRACER_BENCHMARKS_CLI_PARSERS_TRAVERSAL_PARSER_HPP


#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>
#include <htracer_benchmarks/scene_spec.hpp>

#include <string_view>


namespace htracer::benchmarks::cli::parsers
{

struct traversal_parser
{
  [[nodiscard]]
  cli_structure::value_parsing::parse_result<traversal_scene>
  operator()(std::string_view text) const;
};

} // namespace htracer::benchmarks::cli::parsers

#endif
