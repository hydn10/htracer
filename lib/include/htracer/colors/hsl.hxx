#ifndef HTRACER_COLORS_HSL_HXX
#define HTRACER_COLORS_HSL_HXX


#include <htracer/utils/vector_crtp.hpp>


namespace htracer::colors
{

template<typename Float>
class srgb;


template<typename Float>
class hsl final : private utils::vector_crtp<hsl<Float>, Float, 3>
{
public:
  constexpr hsl(Float h, Float s, Float l) noexcept;

  [[nodiscard]]
  constexpr Float const &
  h() const;
  [[nodiscard]]
  constexpr Float const &
  s() const;
  [[nodiscard]]
  constexpr Float const &
  l() const;

  [[nodiscard]]
  constexpr srgb<Float>
  to_srgb() const;
};

} // namespace htracer::colors

#endif
