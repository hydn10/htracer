#ifndef HTRACER_COLORS_SRGBIMPL_HXX
#define HTRACER_COLORS_SRGBIMPL_HXX


#include "srgb.hxx"        // IWYU pragma: export
#include "srgb_linear.hxx" // IWYU pragma: export

#include <htracer/utils/vector_transform.hpp>

#include <cmath>


namespace htracer::colors
{

template<typename Float>
constexpr srgb<Float>::srgb(Float r, Float g, Float b) noexcept
    : utils::vector_crtp<srgb<Float>, Float, 3>{r, g, b}
{
}


template<typename Float>
constexpr Float const &
srgb<Float>::r() const noexcept
{
  return this->template get<0>();
}


template<typename Float>
constexpr Float const &
srgb<Float>::g() const noexcept
{
  return this->template get<1>();
}


template<typename Float>
constexpr Float const &
srgb<Float>::b() const noexcept
{
  return this->template get<2>();
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
        constexpr auto SRGB_CUTOFF = static_cast<Float>(0.0404482362771082);
        constexpr auto SLOPE = static_cast<Float>(12.92);
        constexpr auto EXP_OFFSET = static_cast<Float>(0.055);
        constexpr auto EXPONENT = static_cast<Float>(2.4);

        if (val <= SRGB_CUTOFF)
        {
          return val / SLOPE;
        }

        auto const base = (val + EXP_OFFSET) / (Float{1} + EXP_OFFSET);
        return std::pow(base, EXPONENT);
      });
}

} // namespace htracer::colors

#endif
