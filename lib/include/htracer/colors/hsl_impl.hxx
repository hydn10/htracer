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
  return this->template get<0>();
}


template<typename Float>
constexpr Float const &
hsl<Float>::s() const noexcept
{
  return this->template get<1>();
}


template<typename Float>
constexpr Float const &
hsl<Float>::l() const noexcept
{
  return this->template get<2>();
}


template<typename Float>
constexpr srgb<Float>
hsl<Float>::to_srgb() const
{
  // https://en.wikipedia.org/wiki/HSL_and_HSV
  // https://www.w3.org/TR/css-color-3/#hsl-color

  Float const chroma = (Float{1} - std::abs(Float{2} * l() - Float{1})) * s();
  Float const hprime = h() / Float{60};
  Float const x = chroma * (Float{1} - std::abs(std::fmod(hprime, Float{2}) - Float{1}));
  Float const m = l() - chroma / Float{2};

  if (Float{0} <= hprime && hprime < Float{1})
  {
    return {chroma + m, x + m, m};
  }

  if (Float{1} <= hprime && hprime < Float{2})
  {
    return {x + m, chroma + m, m};
  }

  if (Float{2} <= hprime && hprime < Float{3})
  {
    return {m, chroma + m, x + m};
  }

  if (Float{3} <= hprime && hprime < Float{4})
  {
    return {m, x + m, chroma + m};
  }

  if (Float{4} <= hprime && hprime < Float{5})
  {
    return {x + m, m, chroma + m};
  }

  return {chroma + m, m, x + m};
}

} // namespace htracer::colors

#endif
