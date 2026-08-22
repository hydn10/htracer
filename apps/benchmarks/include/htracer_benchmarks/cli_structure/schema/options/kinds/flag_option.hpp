#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_SCHEMA_OPTIONS_KINDS_FLAG_OPTION_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_SCHEMA_OPTIONS_KINDS_FLAG_OPTION_HPP


#include <htracer_benchmarks/cli_structure/detail/help_writer.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsing/option_descriptor.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsing/parsed_arguments.hpp>
#include <htracer_benchmarks/cli_structure/schema/options/option_info.hpp>

#include <array>
#include <concepts>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>


namespace htracer::benchmarks::cli_structure::schema
{

template<std::default_initializable Presence>
class flag_option
{
  flag_info info_;

public:
  using result_type = std::optional<Presence>;
  static constexpr std::size_t option_count = 1;

  constexpr explicit flag_option(flag_info info);

  [[nodiscard]]
  constexpr auto
  descriptors() const;

  [[nodiscard]]
  result_type
  evaluate(detail::parsing::parsed_arguments const &values, std::string_view path) const;

  void
  print_help(detail::help_writer &writer) const;
};


template<std::default_initializable Presence>
constexpr flag_option<Presence>::flag_option(flag_info info)
    : info_{info}
{
}


template<std::default_initializable Presence>
constexpr auto
flag_option<Presence>::descriptors() const
{
  return std::array{detail::parsing::option_descriptor::flag(info_.name())};
}


template<std::default_initializable Presence>
typename flag_option<Presence>::result_type
flag_option<Presence>::evaluate(detail::parsing::parsed_arguments const &values, std::string_view) const
{
  return values.contains(info_.name().long_name()) ? result_type{Presence{}} : std::nullopt;
}


template<std::default_initializable Presence>
void
flag_option<Presence>::print_help(detail::help_writer &writer) const
{
  auto const name = info_.name();
  writer.entry(
      "-" + std::string(1, name.short_name()) + ", --" + std::string{name.long_name()},
      std::string{info_.description()});
}

} // namespace htracer::benchmarks::cli_structure::schema

#endif
