#ifndef HTRACER_COLORS_HSLIMPL_HXX
#define HTRACER_COLORS_HSLIMPL_HXX


#include "hsl.hxx"
#include "srgb.hxx"

#include <htracer/utils/vector_transform.hpp>

#include <cmath>


namespace htracer::colors
{
template<typename Float>
constexpr hsl<Float>::hsl(Float r, Float g, Float b) noexcept
    : vector_crtp<srgb<Float>, Float, 3>{r, g, b}
{
}


template<typename Float>
constexpr const Float&
hsl<Float>::h() const
{
  return (*this)[0];
}


template<typename Float>
constexpr const Float&
hsl<Float>::s() const
{
  return (*this)[1];
}


template<typename Float>
constexpr const Float&
hsl<Float>::l() const
{
  return (*this)[2];
}


template<typename Float>
constexpr srgb<Float>
hsl<Float>::to_srgb() const
{
  // https://en.wikipedia.org/wiki/HSL_and_HSV

  const auto srgb_from_hprime =
      [](const auto& hprime, const auto& c, const auto& x) -> srgb<Float> {
    if (0 <= hprime && hprime < 1)
      return {c, x, 0};

    if (1 <= hprime && hprime < 2)
      return {x, c, 0};

    if (2 <= hprime && hprime < 3)
      return {0, c, x};

    if (3 <= hprime && hprime < 4)
      return {0, x, c};

    if (4 <= hprime && hprime < 5)
      return {x, 0, c};

    return {c, 0, x};
  };

  const Float chroma = (1 - std::abs(2 * l() - 1)) * s();
  const Float hprime = h() / 60;
  const Float x = chroma * (1 - std::abs(std::fmod(hprime, 2) - 1));

  const Float m = l() - chroma / 2;
  return srgb_from_hprime(hprime, chroma + m, x + m);
}

} // namespace htracer::colors

#endif // HTRACER_COLORS_HSLIMPL_HXX
