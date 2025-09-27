#ifndef HTRACER_COLORS_SRGB_HXX
#define HTRACER_COLORS_SRGB_HXX


#include <htracer/utils/vector_crtp.hpp>


namespace htracer::colors
{

template<typename Float>
class srgb_linear;


template<typename Float>
class srgb final : private utils::vector_crtp<srgb<Float>, Float, 3>
{
public:
  constexpr srgb(Float r, Float g, Float b) noexcept;

  [[nodiscard]]
  constexpr Float const &
  r() const noexcept;
  [[nodiscard]]
  constexpr Float const &
  g() const noexcept;
  [[nodiscard]]
  constexpr Float const &
  b() const noexcept;

  [[nodiscard]]
  constexpr srgb_linear<Float>
  to_linear() const;
};

} // namespace htracer::colors

#endif
