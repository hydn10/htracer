#ifndef HTRACER_COLORS_SRGB_HXX
#define HTRACER_COLORS_SRGB_HXX


#include <htracer/utils/vector_crtp.hpp>


namespace htracer::colors
{
template<typename Float>
class srgb_linear;


template<typename Float>
class srgb : private vector_crtp<srgb<Float>, Float, 3>
{
public:
  constexpr srgb(Float r, Float g, Float b) noexcept;

  [[nodiscard]] constexpr const Float&
  r() const;
  [[nodiscard]] constexpr const Float&
  g() const;
  [[nodiscard]] constexpr const Float&
  b() const;

  [[nodiscard]] constexpr srgb_linear<Float>
  to_linear() const;
};

} // namespace htracer::colors

#endif // HTRACER_COLORS_SRGB_HXX
