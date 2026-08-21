#ifndef HTRACER_COLORS_SRGBLINEARIMPL_HXX
#define HTRACER_COLORS_SRGBLINEARIMPL_HXX


#include "srgb.hxx"        // IWYU pragma: export
#include "srgb_linear.hxx" // IWYU pragma: export

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
        constexpr auto LINEAR_CUTOFF = static_cast<Float>(0.00313066844250063);
        constexpr auto SLOPE = static_cast<Float>(12.92);
        constexpr auto EXP_OFFSET = static_cast<Float>(0.055);
        constexpr auto EXPONENT = static_cast<Float>(2.4);

        if (val <= LINEAR_CUTOFF)
        {
          return val * SLOPE;
        }

        auto const raised = std::pow(val, Float{1} / EXPONENT);
        return (Float{1} + EXP_OFFSET) * raised - EXP_OFFSET;
      });
}


template<typename Float>
constexpr srgb_linear<Float>
operator+(srgb_linear<Float> lhs, srgb_linear<Float> const &rhs) noexcept
{
  return lhs += rhs;
}


template<typename Float>
constexpr srgb_linear<Float>
operator-(srgb_linear<Float> lhs, srgb_linear<Float> const &rhs) noexcept
{
  return lhs -= rhs;
}


template<typename Float, typename TConv>
constexpr srgb_linear<Float>
operator*(srgb_linear<Float> lhs, TConv scale) noexcept
{
  return lhs *= scale;
}


template<typename Float, typename TConv>
constexpr srgb_linear<Float>
operator*(TConv scale, srgb_linear<Float> rhs) noexcept
{
  return rhs *= scale;
}

} // namespace htracer::colors

#endif
