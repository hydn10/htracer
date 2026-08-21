#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_CONTAINS_DUPLICATES_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_CONTAINS_DUPLICATES_HPP


#include <algorithm>
#include <functional>


namespace htracer::benchmarks::cli_structure::detail
{

template<typename Range, typename Projection = std::identity>
[[nodiscard]]
constexpr bool
contains_duplicates(Range const &values, Projection projection = {})
{
  auto sorted = values;
  std::ranges::sort(sorted, {}, projection);
  return std::ranges::adjacent_find(sorted, {}, projection) != sorted.end();
}

} // namespace htracer::benchmarks::cli_structure::detail

#endif
