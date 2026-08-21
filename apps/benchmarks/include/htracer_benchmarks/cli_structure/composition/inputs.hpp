#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_COMPOSITION_INPUTS_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_COMPOSITION_INPUTS_HPP


#include <htracer_benchmarks/cli_structure/detail/brace_constructible.hpp>
#include <htracer_benchmarks/cli_structure/detail/concat_arrays.hpp>
#include <htracer_benchmarks/cli_structure/detail/contains_duplicates.hpp>
#include <htracer_benchmarks/cli_structure/detail/help_writer.hpp>
#include <htracer_benchmarks/cli_structure/detail/option_descriptor.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsed_arguments.hpp>
#include <htracer_benchmarks/cli_structure/foundations/errors.hpp>

#include <array>
#include <cstddef>
#include <functional>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>


namespace htracer::benchmarks::cli_structure
{

template<typename... Items>
class inputs
{
  std::tuple<Items...> items_;

public:
  static constexpr std::size_t option_count = (Items::option_count + ... + std::size_t{0});

  template<typename Result>
  static constexpr bool constructs = detail::brace_constructible_from<Result, typename Items::result_type...>;

  constexpr explicit inputs(Items... items);


  [[nodiscard]]
  constexpr auto
  descriptors() const;

  template<typename Result>
  requires(detail::brace_constructible_from<Result, typename Items::result_type...>)
  [[nodiscard]]
  Result
  construct(detail::parsed_arguments const &values, std::string_view path) const;

  template<typename Parser>
  [[nodiscard]]
  std::invoke_result_t<Parser const &, typename Items::result_type...>
  parse_with(Parser const &parser, detail::parsed_arguments const &values, std::string_view path) const;

  void
  print_help(detail::help_writer &writer) const;
};


template<typename... Items>
inputs(Items...) -> inputs<Items...>;


template<typename... Items>
constexpr inputs<Items...>::inputs(Items... items)
    : items_{std::move(items)...}
{
  auto const values = descriptors();
  if (detail::contains_duplicates(values, &detail::option_descriptor::short_name) ||
      detail::contains_duplicates(values, &detail::option_descriptor::long_name))
  {
    throw schema_error{"duplicate short or long option name in an input set"};
  }
}


template<typename... Items>
constexpr auto
inputs<Items...>::descriptors() const
{
  if constexpr (sizeof...(Items) == 0)
  {
    return std::array<detail::option_descriptor, 0>{};
  }
  else
  {
    return std::apply(
        [](auto const &...items)
        {
          return detail::concat_arrays(items.descriptors()...);
        },
        items_);
  }
}


template<typename... Items>
template<typename Result>
requires(detail::brace_constructible_from<Result, typename Items::result_type...>)
Result
inputs<Items...>::construct(detail::parsed_arguments const &values, std::string_view path) const
{
  auto parsed = std::apply(
      [&](auto const &...items)
      {
        return std::tuple{items.evaluate(values, path)...};
      },
      items_);
  return std::apply(
      [](auto &&...arguments)
      {
        return Result{std::forward<decltype(arguments)>(arguments)...};
      },
      std::move(parsed));
}


template<typename... Items>
template<typename Parser>
std::invoke_result_t<Parser const &, typename Items::result_type...>
inputs<Items...>::parse_with(Parser const &parser, detail::parsed_arguments const &values, std::string_view path) const
{
  auto parsed = std::apply(
      [&](auto const &...items)
      {
        return std::tuple{items.evaluate(values, path)...};
      },
      items_);
  return std::apply(parser, std::move(parsed));
}


template<typename... Items>
void
inputs<Items...>::print_help(detail::help_writer &writer) const
{
  std::apply(
      [&](auto const &...items)
      {
        (items.print_help(writer), ...);
      },
      items_);
}

} // namespace htracer::benchmarks::cli_structure

#endif
