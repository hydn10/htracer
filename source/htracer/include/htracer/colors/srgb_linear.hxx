#ifndef HTRACER_COLORS_SRGBLINEAR_HXX
#define HTRACER_COLORS_SRGBLINEAR_HXX


#include <htracer/utils/vector_crtp.hpp>


namespace htracer::colors
{
template<typename Float>
class srgb;

template<typename Float>
class srgb_linear : private vector_crtp<srgb_linear<Float>, Float, 3>
{
  using VecCrtp = vector_crtp<srgb_linear<Float>, Float, 3>;
  friend VecCrtp;

public:
  using typename VecCrtp::float_type;
  using VecCrtp::size;

  constexpr srgb_linear(Float r, Float g, Float b) noexcept;

  using VecCrtp::operator[];

  using VecCrtp::operator+=;
  using VecCrtp::operator-=;
  using VecCrtp::operator*=;

  [[nodiscard]] srgb<Float>
  to_srgb() const;
};


template<typename Float>
constexpr srgb_linear<Float>
operator+(srgb_linear<Float> lhs, const srgb_linear<Float>& rhs);

template<typename Float>
constexpr srgb_linear<Float>
operator-(srgb_linear<Float> lhs, const srgb_linear<Float>& rhs);

template<typename Float, typename TConv>
constexpr srgb_linear<Float>
operator*(srgb_linear<Float> lhs, TConv scale);

template<typename Float, typename TConv>
srgb_linear<Float>
operator*(TConv scale, srgb_linear<Float> rhs);

} // namespace htracer::colors

#endif // HTRACER_COLORS_SRGBLINEAR_HXX
