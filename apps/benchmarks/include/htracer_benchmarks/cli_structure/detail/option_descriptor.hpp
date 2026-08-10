#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_OPTION_DESCRIPTOR_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_OPTION_DESCRIPTOR_HPP


#include <optional>
#include <string_view>


namespace htracer::benchmarks::cli_structure::detail
{

enum class option_syntax
{
  flag,
  value,
};


struct option_descriptor
{
  char short_name{};
  std::string_view long_name;
  std::string_view value_name;
  option_syntax syntax{};
};


struct option_occurrence
{
  std::string_view long_name;
  std::optional<std::string_view> value;
};


struct parsed_token
{
  std::optional<char> short_name;
  std::string_view long_name;
  std::optional<std::string_view> attached_value;
};

} // namespace htracer::benchmarks::cli_structure::detail

#endif
