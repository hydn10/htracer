#ifndef HTRACER_COLORS_HSLIMPL_HXX
#define HTRACER_COLORS_HSLIMPL_HXX


#include "hsl.hxx"
#include "srgb.hxx"

#include <htracer/utils/vector_transform.hpp>

#include <cmath>


namespace htracer::colors
{

template<typename Float>
constexpr hsl<Float>::hsl(Float h, Float s, Float l) noexcept
    : utils::vector_crtp<srgb<Float>, Float, 3>{h, s, l}
{
}


template<typename Float>
constexpr Float const &
hsl<Float>::h() const
{
  return (*this)[0];
}


template<typename Float>
constexpr Float const &
hsl<Float>::s() const
{
  return (*this)[1];
}


template<typename Float>
constexpr Float const &
hsl<Float>::l() const
{
  return (*this)[2];
}


template<typename Float>
constexpr srgb<Float>
hsl<Float>::to_srgb() const
{
  // https://en.wikipedia.org/wiki/HSL_and_HSV

  auto const srgb_from_hprime = [](auto const &hprime, auto const &c, auto const &x) -> srgb<Float>
  {
    if (0 <= hprime && hprime < 1)
    {
      return {c, x, 0};
    }

    if (1 <= hprime && hprime < 2)
    {
      return {x, c, 0};
    }

    if (2 <= hprime && hprime < 3)
    {
      return {0, c, x};
    }

    if (3 <= hprime && hprime < 4)
    {
      return {0, x, c};
    }

    if (4 <= hprime && hprime < 5)
    {
      return {x, 0, c};
    }

    return {c, 0, x};
  };

  Float const chroma = (1 - std::abs(2 * l() - 1)) * s();
  Float const hprime = h() / 60;
  Float const x = chroma * (1 - std::abs(std::fmod(hprime, 2) - 1));

  Float const m = l() - chroma / 2;
  return srgb_from_hprime(hprime, chroma + m, x + m);
}

} // namespace htracer::colors

#endif
