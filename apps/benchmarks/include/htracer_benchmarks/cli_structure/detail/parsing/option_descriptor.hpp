#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_PARSING_OPTION_DESCRIPTOR_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_PARSING_OPTION_DESCRIPTOR_HPP


#include <htracer_benchmarks/cli_structure/schema/options/option_info.hpp>

#include <cstdint>


namespace htracer::benchmarks::cli_structure::detail::parsing
{

using ::htracer::benchmarks::cli_structure::schema::option_name;

enum class option_arity : std::uint8_t
{
  none,
  one,
};


class option_descriptor
{
  option_name name_;
  option_arity arity_;

  constexpr option_descriptor(option_name name, option_arity arity) noexcept;

public:
  [[nodiscard]]
  static constexpr option_descriptor
  flag(option_name name) noexcept;

  [[nodiscard]]
  static constexpr option_descriptor
  value(option_name name) noexcept;

  [[nodiscard]]
  constexpr option_name
  name() const noexcept;

  [[nodiscard]]
  constexpr bool
  takes_value() const noexcept;
};


constexpr option_descriptor::option_descriptor(option_name name, option_arity arity) noexcept
    : name_{name}
    , arity_{arity}
{
}


constexpr option_descriptor
option_descriptor::flag(option_name name) noexcept
{
  return option_descriptor{name, option_arity::none};
}


constexpr option_descriptor
option_descriptor::value(option_name name) noexcept
{
  return option_descriptor{name, option_arity::one};
}


constexpr option_name
option_descriptor::name() const noexcept
{
  return name_;
}


constexpr bool
option_descriptor::takes_value() const noexcept
{
  return arity_ == option_arity::one;
}

} // namespace htracer::benchmarks::cli_structure::detail::parsing

#endif
