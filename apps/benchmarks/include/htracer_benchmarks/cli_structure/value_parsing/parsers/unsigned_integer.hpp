#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_VALUE_PARSING_PARSERS_UNSIGNED_INTEGER_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_VALUE_PARSING_PARSERS_UNSIGNED_INTEGER_HPP


#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>

#include <charconv>
#include <concepts>
#include <string_view>
#include <system_error>


namespace htracer::benchmarks::cli_structure::value_parsing
{

template<std::unsigned_integral UInt>
struct unsigned_integer
{
  using result_type = UInt;

  [[nodiscard]]
  parse_result<UInt>
  operator()(std::string_view text) const;
};


template<std::unsigned_integral UInt>
parse_result<UInt>
unsigned_integer<UInt>::operator()(std::string_view text) const
{
  UInt value{};

  auto const *begin = text.data();
  auto const *finish = begin + text.size(); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  auto const [end, error] = std::from_chars(begin, finish, value);

  if (error == std::errc::result_out_of_range)
  {
    return std::unexpected{parse_error{"unsigned integer is out of range"}};
  }
  if (error != std::errc{} || end != finish)
  {
    return std::unexpected{parse_error{"expected an unsigned decimal integer"}};
  }
  return value;
}

} // namespace htracer::benchmarks::cli_structure::value_parsing

#endif
