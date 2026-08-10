#include <htracer_benchmarks/cli_structure/detail/parsed_arguments.hpp>

#include <htracer_benchmarks/cli_structure/detail/option_descriptor.hpp>

#include <string_view>
#include <utility>
#include <vector>


namespace htracer::benchmarks::cli_structure::detail
{

parsed_arguments::parsed_arguments(std::vector<option_occurrence> values)
    : values_{std::move(values)}
{
}


option_occurrence const *
parsed_arguments::find(std::string_view name) const noexcept
{
  for (auto const &value : values_)
  {
    if (value.long_name == name)
    {
      return &value;
    }
  }
  return nullptr;
}

} // namespace htracer::benchmarks::cli_structure::detail
