#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_OPTIONS_VALUE_OPTION_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_OPTIONS_VALUE_OPTION_HPP


#include <htracer_benchmarks/cli_structure/detail/help_writer.hpp>
#include <htracer_benchmarks/cli_structure/detail/option_descriptor.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsed_arguments.hpp>
#include <htracer_benchmarks/cli_structure/foundations/errors.hpp>
#include <htracer_benchmarks/cli_structure/foundations/parse_result.hpp>
#include <htracer_benchmarks/cli_structure/options/option_info.hpp>

#include <array>
#include <concepts>
#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>


namespace htracer::benchmarks::cli_structure
{

template<typename Result, typename Parser>
requires std::same_as<std::invoke_result_t<Parser const &, std::string_view>, parse_result<Result>>
class value_option
{
public:
  using result_type = Result;

private:
  value_option_info info_;
  Parser parser_;

  [[nodiscard]]
  result_type
  parse(std::string_view text, std::string_view path) const;

public:
  static constexpr std::size_t option_count = 1;

  constexpr value_option(value_option_info info, Parser parser);

  [[nodiscard]]
  constexpr auto
  descriptors() const;

  [[nodiscard]]
  result_type
  evaluate(detail::parsed_arguments const &values, std::string_view path) const;

  [[nodiscard]]
  std::optional<result_type>
  evaluate_optional(detail::parsed_arguments const &values, std::string_view path) const;

  void
  print_help(detail::help_writer &writer, std::string annotation = " (required)") const;
};


template<typename Parser>
value_option(value_option_info, Parser)
    -> value_option<typename std::invoke_result_t<Parser const &, std::string_view>::value_type, Parser>;

template<typename Result, typename Parser>
requires std::same_as<std::invoke_result_t<Parser const &, std::string_view>, parse_result<Result>>
constexpr value_option<Result, Parser>::value_option(value_option_info info, Parser parser)
    : info_{info}
    , parser_{std::move(parser)}
{
}


template<typename Result, typename Parser>
requires std::same_as<std::invoke_result_t<Parser const &, std::string_view>, parse_result<Result>>
constexpr auto
value_option<Result, Parser>::descriptors() const
{
  return std::array{detail::option_descriptor{
      .short_name = info_.name().short_name(),
      .long_name = info_.name().long_name(),
      .value_name = info_.value_name(),
      .syntax = detail::option_syntax::value}};
}


template<typename Result, typename Parser>
requires std::same_as<std::invoke_result_t<Parser const &, std::string_view>, parse_result<Result>>
value_option<Result, Parser>::result_type
value_option<Result, Parser>::evaluate(detail::parsed_arguments const &values, std::string_view path) const
{
  auto const *value = values.find(info_.name().long_name());
  if (value == nullptr)
  {
    throw usage_error("missing required option: --" + std::string{info_.name().long_name()}, std::string{path});
  }
  return parse(*value->value, path);
}


template<typename Result, typename Parser>
requires std::same_as<std::invoke_result_t<Parser const &, std::string_view>, parse_result<Result>>
std::optional<typename value_option<Result, Parser>::result_type>
value_option<Result, Parser>::evaluate_optional(detail::parsed_arguments const &values, std::string_view path) const
{
  auto const *value = values.find(info_.name().long_name());
  if (value == nullptr)
  {
    return std::nullopt;
  }
  return parse(*value->value, path);
}


template<typename Result, typename Parser>
requires std::same_as<std::invoke_result_t<Parser const &, std::string_view>, parse_result<Result>>
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
requires std::same_as<std::invoke_result_t<Parser const &, std::string_view>, parse_result<Result>>
value_option<Result, Parser>::result_type
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

} // namespace htracer::benchmarks::cli_structure

#endif
