#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_NAME_VALIDATION_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_NAME_VALIDATION_HPP


#include <string_view>


namespace htracer::benchmarks::cli_structure::detail
{

[[nodiscard]]
constexpr bool
is_help(std::string_view value) noexcept;


[[nodiscard]]
constexpr bool
valid_character(char value) noexcept;


[[nodiscard]]
constexpr bool
valid_name(std::string_view value) noexcept;


constexpr bool
is_help(std::string_view value) noexcept
{
  return value == "-h" || value == "--help";
}


constexpr bool
valid_character(char value) noexcept
{
  return (value >= 'a' && value <= 'z') || (value >= '0' && value <= '9') || value == '-';
}


constexpr bool
valid_name(std::string_view value) noexcept
{
  if (value.empty() || value.front() == '-' || value.back() == '-')
  {
    return false;
  }

  for (auto const character : value)
  {
    if (!valid_character(character))
    {
      return false;
    }
  }

  return true;
}

} // namespace htracer::benchmarks::cli_structure::detail

#endif
