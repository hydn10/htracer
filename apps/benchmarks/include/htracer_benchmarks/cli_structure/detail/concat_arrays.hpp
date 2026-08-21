#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_CONCAT_ARRAYS_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_CONCAT_ARRAYS_HPP


#include <algorithm>
#include <array>
#include <cstddef>


namespace htracer::benchmarks::cli_structure::detail
{

template<typename T, std::size_t... Sizes>
[[nodiscard]]
constexpr auto
concat_arrays(std::array<T, Sizes> const &...arrays)
{
  std::array<T, (Sizes + ... + std::size_t{0})> result{};

  // TODO: Use std::views::concat when it is available in all supported standard-library implementations.
  if constexpr (sizeof...(Sizes) > 0)
  {
    auto output = result.begin();
    ((output = std::ranges::copy(arrays, output).out), ...);
  }

  return result;
}

} // namespace htracer::benchmarks::cli_structure::detail

#endif
