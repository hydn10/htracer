#ifndef HTRACER_COLORS_HSL_HXX
#define HTRACER_COLORS_HSL_HXX


#include <htracer/utils/vector_crtp.hpp>


namespace htracer::colors
{

template<typename Float>
class srgb;


template<typename Float>
class hsl : private vector_crtp<srgb<Float>, Float, 3>
{
public:
  constexpr hsl(Float h, Float s, Float l) noexcept;

  [[nodiscard]] constexpr const Float &
  h() const;
  [[nodiscard]] constexpr const Float &
  s() const;
  [[nodiscard]] constexpr const Float &
  l() const;

  [[nodiscard]] constexpr srgb<Float>
  to_srgb() const;
};

} // namespace htracer::colors

#endif