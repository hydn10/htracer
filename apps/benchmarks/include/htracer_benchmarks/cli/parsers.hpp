#ifndef HTRACER_BENCHMARKS_CLI_PARSERS_HPP
#define HTRACER_BENCHMARKS_CLI_PARSERS_HPP


#include <htracer_benchmarks/cli/option_values.hpp>
#include <htracer_benchmarks/cli_structure/foundations/parse_result.hpp>

#include <string_view>


namespace htracer::benchmarks::cli
{

struct width_parser
{
  [[nodiscard]]
  cli_structure::parse_result<width_option>
  operator()(std::string_view text) const;
};


struct height_parser
{
  [[nodiscard]]
  cli_structure::parse_result<height_option>
  operator()(std::string_view text) const;
};


struct precision_parser
{
  [[nodiscard]]
  cli_structure::parse_result<precision_option>
  operator()(std::string_view text) const;
};


struct policy_parser
{
  [[nodiscard]]
  cli_structure::parse_result<policy_option>
  operator()(std::string_view text) const;
};


struct warmups_parser
{
  [[nodiscard]]
  cli_structure::parse_result<warmups_option>
  operator()(std::string_view text) const;
};


struct repetitions_parser
{
  [[nodiscard]]
  cli_structure::parse_result<repetitions_option>
  operator()(std::string_view text) const;
};


struct output_parser
{
  [[nodiscard]]
  cli_structure::parse_result<output_option>
  operator()(std::string_view text) const;
};


struct traversal_parser
{
  [[nodiscard]]
  cli_structure::parse_result<traversal_option>
  operator()(std::string_view text) const;
};


struct samples_parser
{
  [[nodiscard]]
  cli_structure::parse_result<samples_option>
  operator()(std::string_view text) const;
};


struct seed_parser
{
  [[nodiscard]]
  cli_structure::parse_result<seed_option>
  operator()(std::string_view text) const;
};


struct extent_parser
{
  [[nodiscard]]
  cli_structure::parse_result<extent_option>
  operator()(width_option width, height_option height) const;
};

} // namespace htracer::benchmarks::cli

#endif
