#ifndef HTRACER_BENCHMARKS_CLI_PARSERS_DETAIL_HELPERS_HPP
#define HTRACER_BENCHMARKS_CLI_PARSERS_DETAIL_HELPERS_HPP


#include <htracer_benchmarks/cli_structure/value_parsing/parsers/unsigned_integer.hpp>
#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>

#include <expected>
#include <string>
#include <string_view>
#include <utility>


namespace htracer::benchmarks::cli::parsers::detail
{

template<typename Value>
[[nodiscard]]
cli_structure::value_parsing::parse_result<Value>
parse_unsigned(std::string_view text);


template<typename Value>
[[nodiscard]]
cli_structure::value_parsing::parse_result<Value>
parse_unsigned(std::string_view text)
{
  return cli_structure::value_parsing::unsigned_integer<Value>{}(text);
}


template<typename Value>
[[nodiscard]]
cli_structure::value_parsing::parse_result<Value>
as_parse_result(std::expected<Value, std::string_view> result);


template<typename Value>
[[nodiscard]]
cli_structure::value_parsing::parse_result<Value>
as_parse_result(std::expected<Value, std::string_view> result)
{
  if (!result)
  {
    return std::unexpected{cli_structure::value_parsing::parse_error{std::string{result.error()}}};
  }

  return std::move(*result);
}

} // namespace htracer::benchmarks::cli::parsers::detail

#endif
