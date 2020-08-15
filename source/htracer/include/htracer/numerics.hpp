#ifndef HTRACER_NUMERICS_HPP
#define HTRACER_NUMERICS_HPP


#include <htracer/utils/vector_transform.hpp>
#include <htracer/vector.hpp>

#include <algorithm>


namespace htracer
{
template<typename Vector>
constexpr Vector
clamp(
    const Vector& v,
    typename Vector::float_type min,
    typename Vector::float_type max) noexcept
{
  return utils::transform(
      v, [min, max](const auto& val) { return std::clamp(val, min, max); });
}


template<typename Vector>
constexpr Vector
saturate(const Vector& v) noexcept
{
  using float_type = typename Vector::float_type;
  return clamp(v, float_type{0}, float_type{1});
}


template<typename Vector, typename Float>
Vector
pow(const Vector& v, Float exp) noexcept
{
  return utils::transform(
      v, [exp](const auto& val) { return std::pow(val, exp); });
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
reflect(const vector<Float, N>& incident, const vector<Float, N>& normal)
{
  return incident - 2 * dot(incident, normal) * normal;
}

} // namespace htracer

#endif // HTRACER_NUMERICS_HPP
