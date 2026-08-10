#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_PARSING_PARSERS_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_PARSING_PARSERS_HPP


#include <htracer_benchmarks/cli_structure/foundations/parse_result.hpp>

#include <charconv>
#include <concepts>
#include <functional>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <utility>


namespace htracer::benchmarks::cli_structure
{

template<std::unsigned_integral UInt>
struct unsigned_integer
{
  using result_type = UInt;

  [[nodiscard]]
  parse_result<UInt>
  operator()(std::string_view text) const;
};


struct string_value
{
  using result_type = std::string;

  [[nodiscard]]
  parse_result<std::string>
  operator()(std::string_view text) const;
};


template<typename Parser, typename Transform>
class transformed_parser
{
  Parser parser_;
  Transform transform_;

public:
  using result_type = std::invoke_result_t<Transform const &, typename Parser::result_type>;

  constexpr transformed_parser(Parser parser, Transform transform);

  [[nodiscard]]
  parse_result<result_type>
  operator()(std::string_view text) const;
};


template<typename Parser, typename Transform>
[[nodiscard]]
constexpr auto
transform(Parser parser, Transform operation);


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


template<typename Parser, typename Transform>
constexpr transformed_parser<Parser, Transform>::transformed_parser(Parser parser, Transform transform)
    : parser_{std::move(parser)}
    , transform_{std::move(transform)}
{
}


template<typename Parser, typename Transform>
parse_result<typename transformed_parser<Parser, Transform>::result_type>
transformed_parser<Parser, Transform>::operator()(std::string_view text) const
{
  return std::invoke(parser_, text).transform(transform_);
}


template<typename Parser, typename Transform>
constexpr auto
transform(Parser parser, Transform operation)
{
  return transformed_parser<Parser, Transform>{std::move(parser), std::move(operation)};
}

} // namespace htracer::benchmarks::cli_structure

#endif
