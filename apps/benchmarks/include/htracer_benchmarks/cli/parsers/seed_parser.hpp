#ifndef HTRACER_BENCHMARKS_CLI_PARSERS_SEED_PARSER_HPP
#define HTRACER_BENCHMARKS_CLI_PARSERS_SEED_PARSER_HPP


#include <htracer/rendering/random_seed.hpp>
#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>

#include <string_view>


namespace htracer::benchmarks::cli::parsers
{

struct seed_parser
{
  [[nodiscard]]
  cli_structure::value_parsing::parse_result<htracer::rendering::random_seed>
  operator()(std::string_view text) const;
};

} // namespace htracer::benchmarks::cli::parsers

#endif
