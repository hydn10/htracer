#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_OPTION_PARSER_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_OPTION_PARSER_HPP


#include <htracer_benchmarks/cli_structure/detail/argument_view.hpp>
#include <htracer_benchmarks/cli_structure/detail/option_descriptor.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsed_arguments.hpp>
#include <htracer_benchmarks/cli_structure/foundations/errors.hpp>

#include <algorithm>
#include <array>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>


namespace htracer::benchmarks::cli_structure::detail
{

[[nodiscard]]
parsed_token
parse_token(std::string_view argument, std::string_view path);


template<std::size_t Size>
[[nodiscard]]
option_descriptor const &
find_descriptor(
    parsed_token const &token,
    std::array<option_descriptor, Size> const &descriptors,
    std::string_view argument,
    std::string_view path)
{
  auto const descriptor = std::ranges::find_if(
      descriptors,
      [&token](option_descriptor const &candidate)
      {
        return token.short_name ? *token.short_name == candidate.short_name
                                : token.long_name == candidate.long_name;
      });
  if (descriptor == descriptors.end())
  {
    throw usage_error("unknown option: " + std::string{argument}, std::string{path});
  }
  return *descriptor;
}


inline void
reject_duplicate(
    std::string_view long_name, std::vector<option_occurrence> const &values, std::string_view path)
{
  if (std::ranges::any_of(
          values,
          [long_name](option_occurrence const &value)
          {
            return value.long_name == long_name;
          }))
  {
    throw usage_error("duplicate option: --" + std::string{long_name}, std::string{path});
  }
}


inline option_occurrence
parse_occurrence(
    parsed_token const &token,
    option_descriptor const &descriptor,
    argument_view &remaining,
    std::string_view path)
{
  auto const long_name = descriptor.long_name;
  if (descriptor.syntax == option_syntax::flag)
  {
    if (token.attached_value)
    {
      throw usage_error("option does not accept a value: --" + std::string{long_name}, std::string{path});
    }
    return {.long_name = long_name, .value = std::nullopt};
  }

  std::string_view value;
  if (token.attached_value)
  {
    value = *token.attached_value;
  }
  else
  {
    if (remaining.empty())
    {
      throw usage_error("missing value for option: --" + std::string{long_name}, std::string{path});
    }
    value = remaining[0];
    remaining = remaining.subspan(1);
    if (value.starts_with('-'))
    {
      throw usage_error("missing value for option: --" + std::string{long_name}, std::string{path});
    }
  }

  if (value.empty())
  {
    throw usage_error("missing value for option: --" + std::string{long_name}, std::string{path});
  }
  return {.long_name = long_name, .value = value};
}


template<std::size_t Size>
[[nodiscard]]
parsed_arguments
parse_options(argument_view arguments, std::array<option_descriptor, Size> const &descriptors, std::string_view path)
{
  std::vector<option_occurrence> values;
  values.reserve(arguments.size());
  while (!arguments.empty())
  {
    auto const argument = arguments[0];
    arguments = arguments.subspan(1);
    auto const token = parse_token(argument, path);
    auto const &descriptor = find_descriptor(token, descriptors, argument, path);
    reject_duplicate(descriptor.long_name, values, path);
    values.push_back(parse_occurrence(token, descriptor, arguments, path));
  }

  return parsed_arguments{std::move(values)};
}

} // namespace htracer::benchmarks::cli_structure::detail

#endif
