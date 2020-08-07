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
clamp(vector<Float, N> v, Float min, Float max) noexcept
{
  for (decltype(N) i = 0; i < N; ++i)
    v[i] = clamp(v[i], min, max);

  return v;
}


template<typename Float>
Float constexpr saturate(Float value) noexcept
{
  return clamp<Float>(value, 0, 1);
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
saturate(vector<Float, N> v) noexcept
{
  return clamp<Float, N>(v, 0, 1);
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
pow(vector<Float, N> v, Float exp) noexcept
{
  for (decltype(N) i = 0; i < N; ++i)
    v[i] = std::pow(v[i], exp);

  return v;
}

} // namespace htracer

#endif // HTRACER_NUMERICS_HPP
