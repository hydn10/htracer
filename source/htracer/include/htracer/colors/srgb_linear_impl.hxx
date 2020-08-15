#ifndef HTRACER_COLORS_SRGBLINEARIMPL_HXX
#define HTRACER_COLORS_SRGBLINEARIMPL_HXX


#include "srgb.hxx"
#include "srgb_linear.hxx"

#include <htracer/utils/vector_transform.hpp>

#include <cmath>


namespace htracer::colors
{
template<typename Float>
constexpr srgb_linear<Float>::srgb_linear(Float r, Float g, Float b) noexcept
    : vector_crtp<srgb_linear<Float>, Float, 3>{r, g, b}
{
}


template<typename Float>
srgb<Float>
srgb_linear<Float>::to_srgb() const
{
  return utils::transform_into<srgb<Float>>(
      *this, [](auto&& val) { return std::pow(val, 1 / Float{2.2}); });
}


template<typename Float>
constexpr srgb_linear<Float>
operator+(srgb_linear<Float> lhs, const srgb_linear<Float>& rhs)
{
  return lhs += rhs;
}


template<typename Float>
constexpr srgb_linear<Float>
operator-(srgb_linear<Float> lhs, const srgb_linear<Float>& rhs)
{
  return lhs -= rhs;
}


template<typename Float, typename TConv>
constexpr srgb_linear<Float>
operator*(srgb_linear<Float> lhs, TConv scale)
{
  return lhs *= scale;
}


template<typename Float, typename TConv>
srgb_linear<Float>
operator*(TConv scale, srgb_linear<Float> rhs)
{
  return rhs *= scale;
}

} // namespace htracer::colors

#endif // HTRACER_COLORS_SRGBLINEARIMPL_HXX
