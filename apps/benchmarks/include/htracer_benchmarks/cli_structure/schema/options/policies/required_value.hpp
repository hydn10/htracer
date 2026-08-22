#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_SCHEMA_OPTIONS_POLICIES_REQUIRED_VALUE_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_SCHEMA_OPTIONS_POLICIES_REQUIRED_VALUE_HPP


#include <htracer_benchmarks/cli_structure/detail/help_writer.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsing/parsed_arguments.hpp>
#include <htracer_benchmarks/cli_structure/errors.hpp>
#include <htracer_benchmarks/cli_structure/schema/options/kinds/value_option.hpp>

#include <cstddef>
#include <string>
#include <string_view>
#include <utility>


namespace htracer::benchmarks::cli_structure::schema
{

template<value_option_source Option>
class required_value
{
  Option option_;

public:
  using result_type = Option::value_type;
  static constexpr std::size_t option_count = Option::option_count;

  constexpr explicit required_value(Option option);

  [[nodiscard]]
  constexpr auto
  descriptors() const;

  [[nodiscard]]
  result_type
  evaluate(detail::parsing::parsed_arguments const &values, std::string_view path) const;

  void
  print_help(detail::help_writer &writer) const;
};


template<value_option_source Option>
[[nodiscard]]
constexpr auto
required(Option option);


template<value_option_source Option>
constexpr required_value<Option>::required_value(Option option)
    : option_{std::move(option)}
{
}


template<value_option_source Option>
constexpr auto
required_value<Option>::descriptors() const
{
  return option_.descriptors();
}


template<value_option_source Option>
required_value<Option>::result_type
required_value<Option>::evaluate(detail::parsing::parsed_arguments const &values, std::string_view path) const
{
  auto parsed = option_.parse_if_present(values, path);
  if (!parsed)
  {
    throw usage_error("missing required option: --" + std::string{option_.name().long_name()}, std::string{path});
  }
  return std::move(*parsed);
}


template<value_option_source Option>
void
required_value<Option>::print_help(detail::help_writer &writer) const
{
  option_.print_help(writer, " (required)");
}


template<value_option_source Option>
constexpr auto
required(Option option)
{
  return required_value<Option>{std::move(option)};
}

} // namespace htracer::benchmarks::cli_structure::schema

#endif
