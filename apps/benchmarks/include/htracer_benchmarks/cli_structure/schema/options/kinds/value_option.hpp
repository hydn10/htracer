#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_SCHEMA_OPTIONS_KINDS_VALUE_OPTION_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_SCHEMA_OPTIONS_KINDS_VALUE_OPTION_HPP


#include <htracer_benchmarks/cli_structure/detail/help_writer.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsing/option_descriptor.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsing/parsed_arguments.hpp>
#include <htracer_benchmarks/cli_structure/errors.hpp>
#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>
#include <htracer_benchmarks/cli_structure/schema/options/option_info.hpp>

#include <array>
#include <concepts>
#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>


namespace htracer::benchmarks::cli_structure::schema
{

template<typename Result, typename Parser>
requires std::same_as<std::invoke_result_t<Parser const &, std::string_view>, value_parsing::parse_result<Result>>
class value_option
{
  value_option_info info_;
  Parser parser_;

  [[nodiscard]]
  Result
  parse(std::string_view text, std::string_view path) const;

public:
  using value_type = Result;

  static constexpr std::size_t option_count = 1;

  constexpr value_option(value_option_info info, Parser parser);

  [[nodiscard]]
  constexpr auto
  descriptors() const;

  [[nodiscard]]
  constexpr option_name
  name() const noexcept;

  [[nodiscard]]
  std::optional<value_type>
  parse_if_present(detail::parsing::parsed_arguments const &values, std::string_view path) const;

  void
  print_help(detail::help_writer &writer, std::string annotation) const;
};


template<typename Option>
concept value_option_source = requires(
    Option const &option,
    detail::parsing::parsed_arguments const &values,
    std::string_view path,
    detail::help_writer &writer,
    std::string annotation) {
  typename Option::value_type;
  { option.name() } -> std::same_as<option_name>;
  { option.parse_if_present(values, path) } -> std::same_as<std::optional<typename Option::value_type>>;
  option.print_help(writer, std::move(annotation));
};


template<typename Parser>
value_option(value_option_info, Parser)
    -> value_option<typename std::invoke_result_t<Parser const &, std::string_view>::value_type, Parser>;

template<typename Result, typename Parser>
requires std::same_as<std::invoke_result_t<Parser const &, std::string_view>, value_parsing::parse_result<Result>>
constexpr value_option<Result, Parser>::value_option(value_option_info info, Parser parser)
    : info_{info}
    , parser_{std::move(parser)}
{
}


template<typename Result, typename Parser>
requires std::same_as<std::invoke_result_t<Parser const &, std::string_view>, value_parsing::parse_result<Result>>
constexpr auto
value_option<Result, Parser>::descriptors() const
{
  return std::array{detail::parsing::option_descriptor::value(info_.name())};
}


template<typename Result, typename Parser>
requires std::same_as<std::invoke_result_t<Parser const &, std::string_view>, value_parsing::parse_result<Result>>
constexpr option_name
value_option<Result, Parser>::name() const noexcept
{
  return info_.name();
}


template<typename Result, typename Parser>
requires std::same_as<std::invoke_result_t<Parser const &, std::string_view>, value_parsing::parse_result<Result>>
std::optional<typename value_option<Result, Parser>::value_type>
value_option<Result, Parser>::parse_if_present(
    detail::parsing::parsed_arguments const &values, std::string_view path) const
{
  auto const value = values.value(info_.name().long_name());

  if (!value)
  {
    return std::nullopt;
  }

  return parse(*value, path);
}


template<typename Result, typename Parser>
requires std::same_as<std::invoke_result_t<Parser const &, std::string_view>, value_parsing::parse_result<Result>>
void
value_option<Result, Parser>::print_help(detail::help_writer &writer, std::string annotation) const
{
  auto const name = info_.name();

  writer.entry(
      "-" + std::string(1, name.short_name()) + ", --" + std::string{name.long_name()} + " " +
          std::string{info_.value_name()},
      std::string{info_.description()} + std::move(annotation));
}


template<typename Result, typename Parser>
requires std::same_as<std::invoke_result_t<Parser const &, std::string_view>, value_parsing::parse_result<Result>>
value_option<Result, Parser>::value_type
value_option<Result, Parser>::parse(std::string_view text, std::string_view path) const
{
  auto parsed = std::invoke(parser_, text);

  if (!parsed)
  {
    throw usage_error(
        "invalid value for '--" + std::string{info_.name().long_name()} + "': " + parsed.error().message,
        std::string{path});
  }

  return std::move(*parsed);
}


} // namespace htracer::benchmarks::cli_structure::schema

#endif
