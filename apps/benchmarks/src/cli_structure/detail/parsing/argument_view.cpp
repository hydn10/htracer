#include <htracer_benchmarks/cli_structure/detail/parsing/argument_view.hpp>

#include <htracer_benchmarks/cli_structure/detail/name_validation.hpp>

#include <algorithm>
#include <cstddef>
#include <span>
#include <string_view>


namespace htracer::benchmarks::cli_structure::detail::parsing
{

argument_view::argument_view(std::span<char const *const> values) noexcept
    : values_{values}
{
}


bool
argument_view::empty() const noexcept
{
  return values_.empty();
}


std::size_t
argument_view::size() const noexcept
{
  return values_.size();
}


std::string_view
argument_view::operator[](std::size_t index) const noexcept
{
  return values_[index];
}


argument_view
argument_view::subspan(std::size_t offset) const noexcept
{
  return argument_view{values_.subspan(offset)};
}


bool
argument_view::contains_help() const noexcept
{
  return std::ranges::any_of(
      values_,
      [](char const *value)
      {
        return is_help(value);
      });
}

} // namespace htracer::benchmarks::cli_structure::detail::parsing
