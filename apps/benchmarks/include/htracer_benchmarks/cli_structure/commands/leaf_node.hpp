#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_COMMANDS_LEAF_NODE_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_COMMANDS_LEAF_NODE_HPP


#include <htracer_benchmarks/cli_structure/detail/argument_view.hpp>
#include <htracer_benchmarks/cli_structure/detail/brace_constructible.hpp>
#include <htracer_benchmarks/cli_structure/detail/help_writer.hpp>
#include <htracer_benchmarks/cli_structure/detail/option_parser.hpp>
#include <htracer_benchmarks/cli_structure/detail/type_list.hpp>
#include <htracer_benchmarks/cli_structure/foundations/help_page.hpp>

#include <string_view>
#include <utility>


namespace htracer::benchmarks::cli_structure
{

template<typename LeafResult, typename InputSet>
class leaf_node
{
  static_assert(
      InputSet::template constructs<LeafResult>, "a leaf result must be constructible from all of its inputs");

  InputSet values_;

public:
  using result_type = LeafResult;
  using result_types = detail::type_list<LeafResult>;

  constexpr explicit leaf_node(InputSet values);

  template<typename Result>
  requires detail::brace_constructible_from<Result, help_page> &&
           detail::brace_constructible_from<Result, LeafResult>
  [[nodiscard]]
  Result
  parse(detail::argument_view arguments, std::string_view path, std::string_view description) const;

  [[nodiscard]]
  help_page
  render_help(std::string_view path, std::string_view description) const;
};


template<typename Result, typename InputSet>
[[nodiscard]]
constexpr auto
leaf(InputSet values);


template<typename LeafResult, typename InputSet>
constexpr leaf_node<LeafResult, InputSet>::leaf_node(InputSet values)
    : values_{std::move(values)}
{
}


template<typename LeafResult, typename InputSet>
template<typename Result>
requires detail::brace_constructible_from<Result, help_page> &&
         detail::brace_constructible_from<Result, LeafResult>
Result
leaf_node<LeafResult, InputSet>::parse(
    detail::argument_view arguments, std::string_view path, std::string_view description) const
{
  if (arguments.contains_help())
  {
    return Result{render_help(path, description)};
  }

  auto const parsed = detail::parse_options(arguments, values_.descriptors(), path);
  auto result = values_.template construct<LeafResult>(parsed, path);

  return Result{std::move(result)};
}


template<typename LeafResult, typename InputSet>
help_page
leaf_node<LeafResult, InputSet>::render_help(std::string_view path, std::string_view description) const
{
  detail::help_writer writer;

  writer.usage(path, InputSet::option_count == 0 ? "" : " [OPTIONS]");
  writer.paragraph(description);
  writer.heading("Options");

  values_.print_help(writer);
  
  writer.entry("-h, --help", "Show this help");

  return std::move(writer).finish();
}


template<typename Result, typename InputSet>
constexpr auto
leaf(InputSet values)
{
  return leaf_node<Result, InputSet>{std::move(values)};
}

} // namespace htracer::benchmarks::cli_structure

#endif
