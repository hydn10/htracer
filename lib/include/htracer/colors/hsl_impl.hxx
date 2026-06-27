#ifndef HTRACER_COLORS_HSLIMPL_HXX
#define HTRACER_COLORS_HSLIMPL_HXX


#include "hsl.hxx"  // IWYU pragma: export
#include "srgb.hxx" // IWYU pragma: export

#include <htracer/utils/vector_transform.hpp>

#include <cmath>


namespace htracer::colors
{

template<typename Float>
constexpr hsl<Float>::hsl(Float h, Float s, Float l) noexcept
    : utils::vector_crtp<hsl<Float>, Float, 3>{h, s, l}
{
}


template<typename Float>
constexpr Float const &
hsl<Float>::h() const noexcept
{
  return (*this)[0];
}


template<typename Float>
constexpr Float const &
hsl<Float>::s() const noexcept
{
  return (*this)[1];
}


template<typename Float>
constexpr Float const &
hsl<Float>::l() const noexcept
{
  return (*this)[2];
}


template<typename Float>
constexpr srgb<Float>
hsl<Float>::to_srgb() const
{
  // https://en.wikipedia.org/wiki/HSL_and_HSV
  // https://www.w3.org/TR/css-color-3/#hsl-color

  Float const chroma = (1 - std::abs(2 * l() - 1)) * s();
  Float const hprime = h() / 60;
  Float const x = chroma * (1 - std::abs(std::fmod(hprime, 2) - 1));
  Float const m = l() - chroma / 2;

  if (0 <= hprime && hprime < 1)
  {
    return {chroma + m, x + m, m};
  }

  if (1 <= hprime && hprime < 2)
  {
    return {x + m, chroma + m, m};
  }

  if (2 <= hprime && hprime < 3)
  {
    return {m, chroma + m, x + m};
  }

  if (3 <= hprime && hprime < 4)
  {
    return {m, x + m, chroma + m};
  }

  if (4 <= hprime && hprime < 5)
  {
    return {x + m, m, chroma + m};
  }

  return {chroma + m, m, x + m};
}

} // namespace htracer::colors

#endif
