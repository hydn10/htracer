#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_COMPOSITION_OBJECT_INPUT_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_COMPOSITION_OBJECT_INPUT_HPP


#include <htracer_benchmarks/cli_structure/detail/help_writer.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsed_arguments.hpp>

#include <cstddef>
#include <string_view>
#include <utility>


namespace htracer::benchmarks::cli_structure
{

template<typename Result, typename InputSet>
class object_input
{
  static_assert(InputSet::template constructs<Result>, "an object must be constructible from all of its inputs");

  InputSet values_;

public:
  using result_type = Result;
  static constexpr std::size_t option_count = InputSet::option_count;

  constexpr explicit object_input(InputSet values);

  [[nodiscard]]
  constexpr auto
  descriptors() const;

  [[nodiscard]]
  result_type
  evaluate(detail::parsed_arguments const &parsed, std::string_view path) const;

  void
  print_help(detail::help_writer &writer) const;
};


template<typename Result, typename InputSet>
[[nodiscard]]
constexpr auto
object(InputSet values);

template<typename Result, typename InputSet>
constexpr object_input<Result, InputSet>::object_input(InputSet values)
    : values_{std::move(values)}
{
}


template<typename Result, typename InputSet>
constexpr auto
object_input<Result, InputSet>::descriptors() const
{
  return values_.descriptors();
}


template<typename Result, typename InputSet>
object_input<Result, InputSet>::result_type
object_input<Result, InputSet>::evaluate(detail::parsed_arguments const &parsed, std::string_view path) const
{
  return values_.template construct<Result>(parsed, path);
}


template<typename Result, typename InputSet>
void
object_input<Result, InputSet>::print_help(detail::help_writer &writer) const
{
  values_.print_help(writer);
}


template<typename Result, typename InputSet>
constexpr auto
object(InputSet values)
{
  return object_input<Result, InputSet>{std::move(values)};
}

} // namespace htracer::benchmarks::cli_structure

#endif
