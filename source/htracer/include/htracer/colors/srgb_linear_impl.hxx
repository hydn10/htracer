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
    : utils::vector_crtp<srgb_linear<Float>, Float, 3>{r, g, b}
{
}


template<typename Float>
constexpr srgb<Float>
srgb_linear<Float>::to_srgb() const
{
  // https://entropymine.com/imageworsener/srgbformula/

  return utils::transform_into<srgb<Float>>(
      *this,
      [](auto val)
      {
        // TODO: Assert 0 <= val <= 1;
        constexpr Float LINEAR_CUTOFF = 0.00313066844250063;
        constexpr Float SLOPE = 12.92;
        constexpr Float EXP_OFFSET = 0.055;
        constexpr Float EXPONENT = 2.4;

        if (val <= LINEAR_CUTOFF)
          return val * SLOPE;

        const auto raised = std::pow(val, 1 / EXPONENT);
        return (1 + EXP_OFFSET) * raised - EXP_OFFSET;
      });
}


template<typename Float>
constexpr srgb_linear<Float>
operator+(srgb_linear<Float> lhs, srgb_linear<Float> const &rhs)
{
  return lhs += rhs;
}


template<typename Float>
constexpr srgb_linear<Float>
operator-(srgb_linear<Float> lhs, srgb_linear<Float> const &rhs)
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
constexpr srgb_linear<Float>
operator*(TConv scale, srgb_linear<Float> rhs)
{
  return rhs *= scale;
}

} // namespace htracer::colors

#endif