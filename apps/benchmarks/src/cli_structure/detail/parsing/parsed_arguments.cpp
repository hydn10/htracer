#include <htracer_benchmarks/cli_structure/detail/parsing/parsed_arguments.hpp>

#include <algorithm>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>


namespace htracer::benchmarks::cli_structure::detail::parsing
{

parsed_arguments::parsed_arguments(
    std::vector<std::string_view> flags, std::vector<std::pair<std::string_view, std::string_view>> values)
    : flags_{std::move(flags)}
    , values_{std::move(values)}
{
}


bool
parsed_arguments::contains(std::string_view name) const noexcept
{
  return std::ranges::any_of(
             flags_,
             [name](std::string_view flag)
             {
               return flag == name;
             }) ||
         std::ranges::any_of(
             values_,
             [name](auto const &value)
             {
               return value.first == name;
             });
}


std::optional<std::string_view>
parsed_arguments::value(std::string_view name) const noexcept
{
  auto const found = std::ranges::find(values_, name, &decltype(values_)::value_type::first);
  return found == values_.end() ? std::nullopt : std::optional{found->second};
}


} // namespace htracer::benchmarks::cli_structure::detail::parsing
