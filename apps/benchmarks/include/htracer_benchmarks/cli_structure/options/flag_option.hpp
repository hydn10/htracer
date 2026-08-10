#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_OPTIONS_FLAG_OPTION_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_OPTIONS_FLAG_OPTION_HPP


#include <htracer_benchmarks/cli_structure/detail/help_writer.hpp>
#include <htracer_benchmarks/cli_structure/detail/option_descriptor.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsed_arguments.hpp>
#include <htracer_benchmarks/cli_structure/options/option_info.hpp>

#include <array>
#include <concepts>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>


namespace htracer::benchmarks::cli_structure
{

template<typename Presence>
requires std::default_initializable<Presence>
class flag_option
{
  flag_info info_;

public:
  using result_type = std::optional<Presence>;
  static constexpr std::size_t option_count = 1;

  constexpr explicit flag_option(flag_info info);

  template<std::size_t Size>
  constexpr void
  append_descriptors(std::array<detail::option_descriptor, Size> &values, std::size_t &index) const;

  [[nodiscard]]
  result_type
  evaluate(detail::parsed_arguments const &values, std::string_view path) const;

  void
  print_help(detail::help_writer &writer) const;
};


template<typename Presence>
requires std::default_initializable<Presence>
constexpr flag_option<Presence>::flag_option(flag_info info)
    : info_{info}
{
}


template<typename Presence>
requires std::default_initializable<Presence>
template<std::size_t Size>
constexpr void
flag_option<Presence>::append_descriptors(std::array<detail::option_descriptor, Size> &values, std::size_t &index) const
{
  values[index++] = {
      .short_name = info_.name().short_name(),
      .long_name = info_.name().long_name(),
      .value_name = {},
      .syntax = detail::option_syntax::flag};
}


template<typename Presence>
requires std::default_initializable<Presence>
typename flag_option<Presence>::result_type
flag_option<Presence>::evaluate(detail::parsed_arguments const &values, std::string_view) const
{
  return values.find(info_.name().long_name()) == nullptr ? std::nullopt : result_type{Presence{}};
}


template<typename Presence>
requires std::default_initializable<Presence>
void
flag_option<Presence>::print_help(detail::help_writer &writer) const
{
  auto const name = info_.name();
  writer.entry(
      "-" + std::string(1, name.short_name()) + ", --" + std::string{name.long_name()},
      std::string{info_.description()});
}

} // namespace htracer::benchmarks::cli_structure

#endif
