#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_OPTIONS_OPTIONAL_VALUE_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_OPTIONS_OPTIONAL_VALUE_HPP


#include <htracer_benchmarks/cli_structure/detail/help_writer.hpp>
#include <htracer_benchmarks/cli_structure/detail/option_descriptor.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsed_arguments.hpp>

#include <array>
#include <cstddef>
#include <optional>
#include <string_view>
#include <utility>


namespace htracer::benchmarks::cli_structure
{

template<typename Option>
class optional_value
{
  Option option_;

public:
  using result_type = std::optional<typename Option::result_type>;
  static constexpr std::size_t option_count = Option::option_count;

  constexpr explicit optional_value(Option option);

  template<std::size_t Size>
  constexpr void
  append_descriptors(std::array<detail::option_descriptor, Size> &values, std::size_t &index) const;

  [[nodiscard]]
  result_type
  evaluate(detail::parsed_arguments const &values, std::string_view path) const;

  void
  print_help(detail::help_writer &writer) const;
};


template<typename Option>
[[nodiscard]]
constexpr auto
optional(Option option);

template<typename Option>
constexpr optional_value<Option>::optional_value(Option option)
    : option_{std::move(option)}
{
}


template<typename Option>
template<std::size_t Size>
constexpr void
optional_value<Option>::append_descriptors(
    std::array<detail::option_descriptor, Size> &values, std::size_t &index) const
{
  option_.append_descriptors(values, index);
}


template<typename Option>
typename optional_value<Option>::result_type
optional_value<Option>::evaluate(detail::parsed_arguments const &values, std::string_view path) const
{
  return option_.evaluate_optional(values, path);
}


template<typename Option>
void
optional_value<Option>::print_help(detail::help_writer &writer) const
{
  option_.print_help(writer, "");
}


template<typename Option>
constexpr auto
optional(Option option)
{
  return optional_value<Option>{std::move(option)};
}

} // namespace htracer::benchmarks::cli_structure

#endif
