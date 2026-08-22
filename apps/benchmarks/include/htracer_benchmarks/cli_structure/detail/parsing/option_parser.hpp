#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_PARSING_OPTION_PARSER_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_PARSING_OPTION_PARSER_HPP


#include <htracer_benchmarks/cli_structure/detail/parsing/argument_view.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsing/option_descriptor.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsing/parsed_arguments.hpp>
#include <htracer_benchmarks/cli_structure/errors.hpp>

#include <algorithm>
#include <array>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <variant>


namespace htracer::benchmarks::cli_structure::detail::parsing
{

class option_token
{
  std::variant<char, std::string_view> name_;
  std::optional<std::string_view> attached_value_;

  option_token(char name, std::optional<std::string_view> attached_value);
  option_token(std::string_view name, std::optional<std::string_view> attached_value);

  friend option_token
  parse_token(std::string_view argument, std::string_view path);

public:
  [[nodiscard]]
  bool
  matches(option_descriptor const &descriptor) const;

  [[nodiscard]]
  std::optional<std::string_view>
  attached_value() const noexcept;
};


[[nodiscard]]
option_token
parse_token(std::string_view argument, std::string_view path);


template<std::size_t Size>
[[nodiscard]]
option_descriptor const &
find_descriptor(
    option_token const &token,
    std::array<option_descriptor, Size> const &descriptors,
    std::string_view argument,
    std::string_view path);


template<std::size_t Size>
[[nodiscard]]
parsed_arguments
parse_options(argument_view arguments, std::array<option_descriptor, Size> const &descriptors, std::string_view path);


template<std::size_t Size>
[[nodiscard]]
option_descriptor const &
find_descriptor(
    option_token const &token,
    std::array<option_descriptor, Size> const &descriptors,
    std::string_view argument,
    std::string_view path)
{
  auto const descriptor = std::ranges::find_if(
      descriptors,
      [&token](option_descriptor const &candidate)
      {
        return token.matches(candidate);
      });
  if (descriptor == descriptors.end())
  {
    throw usage_error("unknown option: " + std::string{argument}, std::string{path});
  }
  return *descriptor;
}


template<std::size_t Size>
[[nodiscard]]
parsed_arguments
parse_options(argument_view arguments, std::array<option_descriptor, Size> const &descriptors, std::string_view path)
{
  parsed_arguments::producer values{arguments.size()};
  while (!arguments.empty())
  {
    auto const argument = arguments[0];
    arguments = arguments.subspan(1);
    auto const token = parse_token(argument, path);
    auto const &descriptor = find_descriptor(token, descriptors, argument, path);
    values.record(descriptor, token.attached_value(), arguments, path);
  }

  return std::move(values).finish();
}

} // namespace htracer::benchmarks::cli_structure::detail::parsing

#endif
