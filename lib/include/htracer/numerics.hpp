#ifndef HTRACER_NUMERICS_HPP
#define HTRACER_NUMERICS_HPP


#include <htracer/utils/vector_transform.hpp>
#include <htracer/vector.hpp>

#include <algorithm>


namespace htracer
{

template<typename Vector>
constexpr Vector
clamp(Vector const &v, typename Vector::float_type min, typename Vector::float_type max) noexcept
{
  return utils::transform(v, [min, max](auto const &val) { return std::clamp(val, min, max); });
}


template<typename Vector>
constexpr Vector
saturate(Vector const &v) noexcept
{
  using float_type = typename Vector::float_type;
  return clamp(v, float_type{0}, float_type{1});
}


template<typename Vector, typename Float>
Vector
pow(Vector const &v, Float exp)
{
  return utils::transform(v, [exp](auto const &val) { return std::pow(val, exp); });
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
reflect(vector<Float, N> const &incident, vector<Float, N> const &normal) noexcept
{
  return incident - 2 * dot(incident, normal) * normal;
}

} // namespace htracer

#endif