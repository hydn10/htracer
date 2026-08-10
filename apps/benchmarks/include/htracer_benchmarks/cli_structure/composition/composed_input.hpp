#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_COMPOSITION_COMPOSED_INPUT_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_COMPOSITION_COMPOSED_INPUT_HPP


#include <htracer_benchmarks/cli_structure/detail/help_writer.hpp>
#include <htracer_benchmarks/cli_structure/detail/option_descriptor.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsed_arguments.hpp>
#include <htracer_benchmarks/cli_structure/foundations/errors.hpp>
#include <htracer_benchmarks/cli_structure/foundations/parse_result.hpp>

#include <array>
#include <concepts>
#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>


namespace htracer::benchmarks::cli_structure
{

template<typename Result, typename InputSet, typename Parser>
requires std::same_as<
    decltype(std::declval<InputSet const &>().parse_with(
        std::declval<Parser const &>(),
        std::declval<detail::parsed_arguments const &>(),
        std::declval<std::string_view>())),
    parse_result<Result>>
class composed_input
{
  InputSet values_;
  Parser parser_;

public:
  using result_type = Result;
  static constexpr std::size_t option_count = InputSet::option_count;

  constexpr composed_input(InputSet values, Parser parser);

  template<std::size_t Size>
  constexpr void
  append_descriptors(std::array<detail::option_descriptor, Size> &result, std::size_t &index) const;

  [[nodiscard]]
  result_type
  evaluate(detail::parsed_arguments const &parsed, std::string_view path) const;

  void
  print_help(detail::help_writer &writer) const;
};


template<typename Result, typename InputSet, typename Parser>
[[nodiscard]]
constexpr auto
compose(InputSet values, Parser parser);

template<typename Result, typename InputSet, typename Parser>
requires std::same_as<
             decltype(std::declval<InputSet const &>().parse_with(
                 std::declval<Parser const &>(),
                 std::declval<detail::parsed_arguments const &>(),
                 std::declval<std::string_view>())),
             parse_result<Result>>
constexpr composed_input<Result, InputSet, Parser>::composed_input(InputSet values, Parser parser)
    : values_{std::move(values)}
    , parser_{std::move(parser)}
{
}


template<typename Result, typename InputSet, typename Parser>
requires std::same_as<
    decltype(std::declval<InputSet const &>().parse_with(
        std::declval<Parser const &>(),
        std::declval<detail::parsed_arguments const &>(),
        std::declval<std::string_view>())),
    parse_result<Result>>
template<std::size_t Size>
constexpr void
composed_input<Result, InputSet, Parser>::append_descriptors(
    std::array<detail::option_descriptor, Size> &result, std::size_t &index) const
{
  values_.append_descriptors(result, index);
}


template<typename Result, typename InputSet, typename Parser>
requires std::same_as<
    decltype(std::declval<InputSet const &>().parse_with(
        std::declval<Parser const &>(),
        std::declval<detail::parsed_arguments const &>(),
        std::declval<std::string_view>())),
    parse_result<Result>>
typename composed_input<Result, InputSet, Parser>::result_type
composed_input<Result, InputSet, Parser>::evaluate(detail::parsed_arguments const &parsed, std::string_view path) const
{
  auto result = values_.parse_with(parser_, parsed, path);
  if (!result)
  {
    throw usage_error("invalid option combination: " + result.error().message, std::string{path});
  }
  return std::move(*result);
}


template<typename Result, typename InputSet, typename Parser>
requires std::same_as<
    decltype(std::declval<InputSet const &>().parse_with(
        std::declval<Parser const &>(),
        std::declval<detail::parsed_arguments const &>(),
        std::declval<std::string_view>())),
    parse_result<Result>>
void
composed_input<Result, InputSet, Parser>::print_help(detail::help_writer &writer) const
{
  values_.print_help(writer);
}


template<typename Result, typename InputSet, typename Parser>
constexpr auto
compose(InputSet values, Parser parser)
{
  return composed_input<Result, InputSet, Parser>{std::move(values), std::move(parser)};
}

} // namespace htracer::benchmarks::cli_structure

#endif
