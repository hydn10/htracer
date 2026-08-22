#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_SCHEMA_COMMAND_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_SCHEMA_COMMAND_HPP


#include <htracer_benchmarks/cli_structure/detail/parsing/argument_view.hpp>
#include <htracer_benchmarks/cli_structure/detail/name_validation.hpp>
#include <htracer_benchmarks/cli_structure/errors.hpp>

#include <string>
#include <string_view>
#include <utility>


namespace htracer::benchmarks::cli_structure::schema
{

template<typename Body>
class command
{
  std::string_view name_;
  std::string_view description_;
  Body body_;

public:
  using result_types = Body::result_types;

  constexpr command(std::string_view name, std::string_view description, Body body);

  [[nodiscard]]
  constexpr std::string_view
  name() const noexcept;

  [[nodiscard]]
  constexpr std::string_view
  description() const noexcept;

  template<typename Result>
  [[nodiscard]]
  Result
  parse(detail::parsing::argument_view arguments, std::string_view path) const;
};


template<typename Body>
command(std::string_view, std::string_view, Body) -> command<Body>;


template<typename Body>
constexpr command<Body>::command(std::string_view name, std::string_view description, Body body)
    : name_{name}
    , description_{description}
    , body_{std::move(body)}
{
  if (!detail::valid_name(name) || description.empty())
  {
    throw schema_error{"invalid command name or description"};
  }
}


template<typename Body>
constexpr std::string_view
command<Body>::name() const noexcept
{
  return name_;
}


template<typename Body>
constexpr std::string_view
command<Body>::description() const noexcept
{
  return description_;
}


template<typename Body>
template<typename Result>
Result
command<Body>::parse(detail::parsing::argument_view arguments, std::string_view path) const
{
  return body_.template parse<Result>(arguments, path, description_);
}

} // namespace htracer::benchmarks::cli_structure::schema

#endif
