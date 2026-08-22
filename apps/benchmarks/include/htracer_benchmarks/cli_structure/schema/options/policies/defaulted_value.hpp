#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_SCHEMA_OPTIONS_POLICIES_DEFAULTED_VALUE_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_SCHEMA_OPTIONS_POLICIES_DEFAULTED_VALUE_HPP


#include <htracer_benchmarks/cli_structure/detail/help_writer.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsing/parsed_arguments.hpp>
#include <htracer_benchmarks/cli_structure/schema/options/kinds/value_option.hpp>

#include <concepts>
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>


namespace htracer::benchmarks::cli_structure::schema
{

template<typename Option>
requires value_option_source<Option> && std::copy_constructible<typename Option::value_type>
class defaulted_value
{
  Option option_;
  Option::value_type default_;
  std::string_view text_;

public:
  using result_type = Option::value_type;

  static constexpr std::size_t option_count = Option::option_count;

  constexpr defaulted_value(Option option, result_type default_value, std::string_view default_text);

  [[nodiscard]]
  constexpr auto
  descriptors() const;

  [[nodiscard]]
  result_type
  evaluate(detail::parsing::parsed_arguments const &values, std::string_view path) const;

  void
  print_help(detail::help_writer &writer) const;
};


template<typename Option>
requires value_option_source<Option> && std::copy_constructible<typename Option::value_type>
[[nodiscard]]
constexpr auto
defaulted(Option option, typename Option::value_type value, std::string_view text);

template<typename Option>
requires value_option_source<Option> && std::copy_constructible<typename Option::value_type>
constexpr defaulted_value<Option>::defaulted_value(
    Option option, result_type default_value, std::string_view default_text)
    : option_{std::move(option)}
    , default_{std::move(default_value)}
    , text_{default_text}
{
}


template<typename Option>
requires value_option_source<Option> && std::copy_constructible<typename Option::value_type>
constexpr auto
defaulted_value<Option>::descriptors() const
{
  return option_.descriptors();
}


template<typename Option>
requires value_option_source<Option> && std::copy_constructible<typename Option::value_type>
defaulted_value<Option>::result_type
defaulted_value<Option>::evaluate(detail::parsing::parsed_arguments const &values, std::string_view path) const
{
  auto parsed = option_.parse_if_present(values, path);
  return parsed ? std::move(*parsed) : default_;
}


template<typename Option>
requires value_option_source<Option> && std::copy_constructible<typename Option::value_type>
void
defaulted_value<Option>::print_help(detail::help_writer &writer) const
{
  option_.print_help(writer, " (default: " + std::string{text_} + ")");
}


template<typename Option>
requires value_option_source<Option> && std::copy_constructible<typename Option::value_type>
constexpr auto
defaulted(Option option, typename Option::value_type value, std::string_view text)
{
  return defaulted_value<Option>{std::move(option), std::move(value), text};
}

} // namespace htracer::benchmarks::cli_structure::schema

#endif
