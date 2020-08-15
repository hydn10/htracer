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
const Float&
srgb<Float>::r() const
{
  return (*this)[0];
}


template<typename Float>
const Float&
srgb<Float>::g() const
{
  return (*this)[1];
}


template<typename Float>
const Float&
srgb<Float>::b() const
{
  return (*this)[2];
}


template<typename Float>
srgb_linear<Float>
srgb<Float>::to_linear() const
{
  return utils::transform_into<srgb_linear<Float>>(
      *this, [](auto&& val) { return std::pow(val, Float{2.2}); });
}

} // namespace htracer::colors

#endif // HTRACER_COLORS_SRGBIMPL_HXX
