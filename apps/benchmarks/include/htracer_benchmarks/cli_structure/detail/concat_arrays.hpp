#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_CONCAT_ARRAYS_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_CONCAT_ARRAYS_HPP


#include <array>
#include <cstddef>
#include <utility>


namespace htracer::benchmarks::cli_structure::detail
{

template<typename T, std::size_t LeftSize, std::size_t RightSize, std::size_t... Left, std::size_t... Right>
[[nodiscard]]
constexpr auto
concat_arrays_impl(
    std::array<T, LeftSize> const &left,
    std::array<T, RightSize> const &right,
    [[maybe_unused]] std::index_sequence<Left...> left_indices,
    [[maybe_unused]] std::index_sequence<Right...> right_indices);


template<typename T, std::size_t LeftSize, std::size_t RightSize>
[[nodiscard]]
constexpr auto
concat_arrays(std::array<T, LeftSize> const &left, std::array<T, RightSize> const &right);


template<typename T, std::size_t FirstSize>
[[nodiscard]]
constexpr auto
concat_arrays(std::array<T, FirstSize> const &first);


template<typename T, std::size_t FirstSize, std::size_t SecondSize, std::size_t... RemainingSizes>
[[nodiscard]]
constexpr auto
concat_arrays(
    std::array<T, FirstSize> const &first,
    std::array<T, SecondSize> const &second,
    std::array<T, RemainingSizes> const &...remaining);


template<typename T, std::size_t LeftSize, std::size_t RightSize, std::size_t... Left, std::size_t... Right>
[[nodiscard]]
constexpr auto
concat_arrays_impl(
    std::array<T, LeftSize> const &left,
    std::array<T, RightSize> const &right,
    [[maybe_unused]] std::index_sequence<Left...> left_indices,
    [[maybe_unused]] std::index_sequence<Right...> right_indices)
{
  return std::array<T, LeftSize + RightSize>{left[Left]..., right[Right]...};
}


template<typename T, std::size_t LeftSize, std::size_t RightSize>
[[nodiscard]]
constexpr auto
concat_arrays(std::array<T, LeftSize> const &left, std::array<T, RightSize> const &right)
{
  return concat_arrays_impl(left, right, std::make_index_sequence<LeftSize>{}, std::make_index_sequence<RightSize>{});
}


template<typename T, std::size_t FirstSize>
[[nodiscard]]
constexpr auto
concat_arrays(std::array<T, FirstSize> const &first)
{
  return first;
}


template<typename T, std::size_t FirstSize, std::size_t SecondSize, std::size_t... RemainingSizes>
[[nodiscard]]
constexpr auto
concat_arrays(
    std::array<T, FirstSize> const &first,
    std::array<T, SecondSize> const &second,
    std::array<T, RemainingSizes> const &...remaining)
{
  return concat_arrays(concat_arrays(first, second), remaining...);
}

} // namespace htracer::benchmarks::cli_structure::detail

#endif
