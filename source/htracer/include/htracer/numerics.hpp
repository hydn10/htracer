#ifndef HTRACER_NUMERICS_HPP
#define HTRACER_NUMERICS_HPP


#include <htracer/vector.hpp>

#include <algorithm>
#include <utility>


namespace htracer
{
namespace detail_
{
template<typename T, std::size_t N, typename F, std::size_t... Is>
std::array<T, N>
transform(const vector<T, N>& v, F f, std::index_sequence<Is...>)
{
  return {{f(v[Is])...}};
}

template<typename T, std::size_t N, typename F>
std::array<T, N>
transform(const vector<T, N>& v, F f)
{
  return transform(v, f, std::make_index_sequence<N>());
}

} // namespace detail_

template<typename Float>
Float constexpr clamp(Float value, Float min, Float max) noexcept
{
  return std::clamp<Float>(value, min, max);
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
clamp(const vector<Float, N>& v, Float min, Float max) noexcept
{
  return detail_::transform(
      v, [min, max](auto val) { return clamp(val, min, max); });
}


template<typename Float>
Float constexpr saturate(Float value) noexcept
{
  return clamp<Float>(value, 0, 1);
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
saturate(const vector<Float, N>& v) noexcept
{
  return clamp<Float, N>(v, 0, 1);
}


template<typename Float, std::size_t N>
vector<Float, N>
pow(const vector<Float, N>& v, Float exp) noexcept
{
  return detail_::transform(v, [exp](auto val) { return std::pow(val, exp); });
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
reflect(
    const vector<Float, N>& incident,
    const vector<Float, N>& normal)
{
  return incident - 2 * dot(incident, normal) * normal;
}

} // namespace htracer

#endif // HTRACER_NUMERICS_HPP
