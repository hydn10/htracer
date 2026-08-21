#ifndef HTRACER_VECTOR_HPP
#define HTRACER_VECTOR_HPP


#include <htracer/utils/vector_crtp.hpp>

#include <array>
#include <cmath>
#include <concepts>
#include <numeric>
#include <type_traits>
#include <utility>


namespace htracer
{

template<typename Float, std::size_t N>
class vector;


template<typename Float, std::size_t N>
class vector final : private utils::vector_crtp<vector<Float, N>, Float, N>
{
  using VecCrtp = utils::vector_crtp<vector<Float, N>, Float, N>;
  friend VecCrtp;

public:
  using typename VecCrtp::float_type;
  using VecCrtp::size;

  using iterator = VecCrtp::iterator;
  using const_iterator = VecCrtp::const_iterator;

  constexpr vector() noexcept = default;
  constexpr explicit vector(std::array<Float, N> values) noexcept;
  template<typename... Args>
  requires(sizeof...(Args) == N && (std::constructible_from<Float, Args> && ...))
  explicit(sizeof...(Args) == 1) constexpr vector(Args &&...values) noexcept(
      utils::detail_::vector_components_nothrow<Float, Args...>);

  using VecCrtp::get;

  using VecCrtp::operator+=;
  using VecCrtp::operator-=;
  using VecCrtp::operator*=;

  using VecCrtp::begin;
  using VecCrtp::end;

  using VecCrtp::cbegin;
  using VecCrtp::cend;

  void
  swap(vector<Float, N> &rhs) noexcept;
};


template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator+(vector<Float, N> lhs, vector<Float, N> const &rhs) noexcept;

template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator-(vector<Float, N> lhs, vector<Float, N> const &rhs) noexcept;

template<typename Float, std::size_t N, typename TConv>
constexpr vector<Float, N>
operator*(vector<Float, N> lhs, TConv scale) noexcept;

template<typename Float, std::size_t N, typename TConv>
constexpr vector<Float, N>
operator*(TConv scale, vector<Float, N> rhs) noexcept;

template<typename Float, std::size_t N>
constexpr Float
dot(vector<Float, N> const &lhs, vector<Float, N> const &rhs) noexcept;

template<typename Float, std::size_t N>
[[nodiscard]]
constexpr vector<Float, N>
normalize(vector<Float, N> const &v) noexcept;

template<typename Float>
constexpr vector<Float, 3>
cross(vector<Float, 3> const &lhs, vector<Float, 3> const &rhs) noexcept;


template<typename Float>
using v3 = vector<Float, 3>;


template<typename Float, std::size_t N>
void
vector<Float, N>::swap(vector<Float, N> &rhs) noexcept
{
  VecCrtp::swap(rhs);
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>::vector(std::array<Float, N> values) noexcept
    : VecCrtp{std::move(values)}
{
}


template<typename Float, std::size_t N>
template<typename... Args>
requires(sizeof...(Args) == N && (std::constructible_from<Float, Args> && ...))
constexpr vector<Float, N>::vector(Args &&...values) noexcept(
    utils::detail_::vector_components_nothrow<Float, Args...>)
    : VecCrtp{std::forward<Args>(values)...}
{
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator+(vector<Float, N> lhs, vector<Float, N> const &rhs) noexcept
{
  return lhs += rhs;
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator-(vector<Float, N> lhs, vector<Float, N> const &rhs) noexcept
{
  return lhs -= rhs;
}


template<typename Float, std::size_t N, typename TConv>
constexpr vector<Float, N>
operator*(vector<Float, N> lhs, TConv scale) noexcept
{
  return lhs *= scale;
}


template<typename Float, std::size_t N, typename TConv>
constexpr vector<Float, N>
operator*(TConv scale, vector<Float, N> rhs) noexcept
{
  return rhs *= scale;
}


template<typename Float, std::size_t N>
constexpr Float
dot(vector<Float, N> const &lhs, vector<Float, N> const &rhs) noexcept
{
  return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), Float{0});
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
normalize(vector<Float, N> const &v) noexcept
{
  auto const norm = std::sqrt(dot(v, v));
  return v * (1 / norm);
}


template<typename Float>
constexpr vector<Float, 3>
cross(vector<Float, 3> const &lhs, vector<Float, 3> const &rhs) noexcept
{
  return {
      lhs.template get<1>() * rhs.template get<2>() - rhs.template get<1>() * lhs.template get<2>(),
      lhs.template get<2>() * rhs.template get<0>() - rhs.template get<2>() * lhs.template get<0>(),
      lhs.template get<0>() * rhs.template get<1>() - rhs.template get<0>() * lhs.template get<1>()};
}

} // namespace htracer

#endif
