#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_OPTION_PARSER_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_OPTION_PARSER_HPP


#include <htracer_benchmarks/cli_structure/detail/argument_view.hpp>
#include <htracer_benchmarks/cli_structure/detail/option_descriptor.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsed_arguments.hpp>
#include <htracer_benchmarks/cli_structure/foundations/errors.hpp>

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
parsed_arguments
parse_options(argument_view arguments, std::array<option_descriptor, Size> const &descriptors, std::string_view path);


template<std::size_t Size>
parsed_arguments
parse_options(argument_view arguments, std::array<option_descriptor, Size> const &descriptors, std::string_view path)
{
  std::vector<option_occurrence> values;
  values.reserve(arguments.size());
  for (std::size_t index = 0; index < arguments.size(); ++index)
  {
    auto const argument = arguments[index];
    auto const token = parse_token(argument, path);
    option_descriptor const *descriptor{};
    for (auto const &candidate : descriptors)
    {
      if (token.short_name ? *token.short_name == candidate.short_name : token.long_name == candidate.long_name)
      {
        descriptor = &candidate;
        break;
      }
    }

    if (descriptor == nullptr)
    {
      throw usage_error("unknown option: " + std::string{argument}, std::string{path});
    }

    auto const long_name = descriptor->long_name;
    for (auto const &value : values)
    {
      if (value.long_name == long_name)
      {
        throw usage_error("duplicate option: --" + std::string{long_name}, std::string{path});
      }
    }

    if (descriptor->syntax == option_syntax::flag)
    {
      if (token.attached_value)
      {
        throw usage_error("option does not accept a value: --" + std::string{long_name}, std::string{path});
      }
      values.push_back({.long_name = long_name, .value = std::nullopt});
      continue;
    }

    std::string_view value;
    if (token.attached_value)
    {
      value = *token.attached_value;
    }
    else
    {
      if (index + 1 >= arguments.size())
      {
        throw usage_error("missing value for option: --" + std::string{long_name}, std::string{path});
      }
      value = arguments[++index];
      if (value.starts_with('-'))
      {
        throw usage_error("missing value for option: --" + std::string{long_name}, std::string{path});
      }
    }

    if (value.empty())
    {
      throw usage_error("missing value for option: --" + std::string{long_name}, std::string{path});
    }

    values.push_back({.long_name = long_name, .value = value});
  }

  return parsed_arguments{std::move(values)};
}

} // namespace htracer::benchmarks::cli_structure::detail

#endif
