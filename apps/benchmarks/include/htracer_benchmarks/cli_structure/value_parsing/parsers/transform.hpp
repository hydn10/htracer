#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_VALUE_PARSING_PARSERS_TRANSFORM_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_VALUE_PARSING_PARSERS_TRANSFORM_HPP


#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>

#include <functional>
#include <string_view>
#include <type_traits>
#include <utility>


namespace htracer::benchmarks::cli_structure::value_parsing
{

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

} // namespace htracer::benchmarks::cli_structure::value_parsing

#endif
