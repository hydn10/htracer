#ifndef HTRACER_NUMERICS_HPP
#define HTRACER_NUMERICS_HPP


#include <htracer/vector.hpp>

#include <algorithm>


namespace htracer
{
template<typename Float>
Float constexpr clamp(Float value, Float min, Float max) noexcept
{
  return std::clamp<Float>(value, min, max);
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
clamp(const vector<Float, N>& v, Float min, Float max) noexcept
{
  vector<Float, N> res;

  for (decltype(N) i = 0; i < N; ++i)
    res[i] = clamp(v[i], min, max);

  return res;
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
constexpr vector<Float, N>
pow(vector<Float, N> v, Float exp) noexcept
{
  std::array<Float, N> res;

  for (decltype(N) i = 0; i < N; ++i)
    res[i] = std::pow(v[i], exp);

  return res;
}

} // namespace htracer

#endif // HTRACER_NUMERICS_HPP
