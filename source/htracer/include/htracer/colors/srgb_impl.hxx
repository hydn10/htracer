#ifndef HTRACER_COLORS_SRGBIMPL_HXX
#define HTRACER_COLORS_SRGBIMPL_HXX


#include "srgb.hxx"
#include "srgb_linear.hxx"

#include <htracer/utils/vector_transform.hpp>

#include <cmath>


namespace htracer::colors
{

template<typename Float>
constexpr srgb<Float>::srgb(Float r, Float g, Float b) noexcept
    : vector_crtp<srgb<Float>, Float, 3>{r, g, b}
{
}


template<typename Float>
constexpr const Float &
srgb<Float>::r() const
{
  return (*this)[0];
}


template<typename Float>
constexpr const Float &
srgb<Float>::g() const
{
  return (*this)[1];
}


template<typename Float>
constexpr const Float &
srgb<Float>::b() const
{
  return (*this)[2];
}


template<typename Float>
constexpr srgb_linear<Float>
srgb<Float>::to_linear() const
{
  // https://entropymine.com/imageworsener/srgbformula/

  return utils::transform_into<srgb_linear<Float>>(
      *this,
      [](auto val)
      {
        // TODO: Assert 0 <= val <= 1;
        constexpr Float SRGB_CUTOFF = 0.0404482362771082;
        constexpr Float SLOPE = 12.92;
        constexpr Float EXP_OFFSET = 0.055;
        constexpr Float EXPONENT = 2.4;

        if (val <= SRGB_CUTOFF)
          return val / SLOPE;

        const auto base = (val + EXP_OFFSET) / (1 + EXP_OFFSET);
        return std::pow(base, EXPONENT);
      });
}

} // namespace htracer::colors

#endif