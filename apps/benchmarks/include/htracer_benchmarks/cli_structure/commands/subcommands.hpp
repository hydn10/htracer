#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_COMMANDS_SUBCOMMANDS_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_COMMANDS_SUBCOMMANDS_HPP


#include <htracer_benchmarks/cli_structure/detail/argument_view.hpp>
#include <htracer_benchmarks/cli_structure/detail/help_writer.hpp>
#include <htracer_benchmarks/cli_structure/detail/name_validation.hpp>
#include <htracer_benchmarks/cli_structure/detail/type_list.hpp>
#include <htracer_benchmarks/cli_structure/foundations/errors.hpp>
#include <htracer_benchmarks/cli_structure/foundations/help_page.hpp>

#include <array>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>


namespace htracer::benchmarks::cli_structure
{

template<typename... Children>
class subcommands
{
  static_assert(sizeof...(Children) > 0, "a subcommand group cannot be empty");

  std::tuple<Children...> children_;

public:
  using result_types = typename detail::concatenate<typename Children::result_types...>::type;

  constexpr explicit subcommands(Children... children);

  template<typename Result>
  [[nodiscard]]
  Result
  parse(detail::argument_view arguments, std::string_view path, std::string_view description) const;

  [[nodiscard]]
  help_page
  render_help(std::string_view path, std::string_view description) const;
};


template<typename... Children>
subcommands(Children...) -> subcommands<Children...>;


template<typename... Children>
constexpr subcommands<Children...>::subcommands(Children... children)
    : children_{std::move(children)...}
{
  auto const names = std::apply(
      [](auto const &...values)
      {
        return std::array{values.name()...};
      },
      children_);

  for (std::size_t outer = 0; outer < names.size(); ++outer)
  {
    for (std::size_t inner = outer + 1; inner < names.size(); ++inner)
    {
      if (names[outer] == names[inner])
      {
        throw schema_error{"duplicate sibling command name"};
      }
    }
  }
}


template<typename... Children>
template<typename Result>
Result
subcommands<Children...>::parse(
    detail::argument_view arguments, std::string_view path, std::string_view description) const
{
  if (!arguments.empty() && detail::is_help(arguments[0]))
  {
    return Result{render_help(path, description)};
  }

  if (arguments.empty())
  {
    throw usage_error("missing subcommand for '" + std::string{path} + "'", std::string{path});
  }

  auto const candidate = arguments[0];
  auto const remaining = arguments.subspan(1);

  auto parsed = std::apply(
      [&](auto const &...children) -> std::optional<Result>
      {
        std::optional<Result> result;

        auto try_parse = [&](auto const &child)
        {
          if (candidate != child.name())
          {
            return false;
          }

          result.emplace(child.template parse<Result>(remaining, std::string{path} + " " + std::string{child.name()}));
          return true;
        };

        (try_parse(children) || ...);
        return result;
      },
      children_);

  if (!parsed)
  {
    throw usage_error(
        "unknown subcommand for '" + std::string{path} + "': " + std::string{candidate}, std::string{path});
  }

  return std::move(*parsed);
}


template<typename... Children>
help_page
subcommands<Children...>::render_help(std::string_view path, std::string_view description) const
{
  detail::help_writer writer;
  writer.usage(path, " <command>");
  writer.paragraph(description);
  writer.heading("Commands");
  std::apply(
      [&](auto const &...children)
      {
        (writer.entry(std::string{children.name()}, std::string{children.description()}), ...);
      },
      children_);
  writer.paragraph("Append --help to a command path for contextual help.");
  return std::move(writer).finish();
}

} // namespace htracer::benchmarks::cli_structure

#endif
