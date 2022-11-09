#ifndef HTRACER_RAYTRACING_LENSES_PINHOLE__HPP
#define HTRACER_RAYTRACING_LENSES_PINHOLE_HPP


#include <htracer/utils/random.hpp>
#include <htracer/vector.hpp>
#include <htracer/geometries/ray.hpp>

#include <numbers>


namespace htracer::raytracing::lenses
{

template<typename Float>
class pinhole
{
  Float aperture_radius_;
  Float focal_distance_;

  utils::random<Float> rand_unit_;

public:
  pinhole(Float aperture_radius, Float focal_distance)
      : aperture_radius_{aperture_radius}
      , focal_distance_{focal_distance}
      , rand_unit_{0, 1}
  {
  }

  geometries::ray<Float>
  get_ray(
      Float dv,
      Float dh,
      v3<Float> const &position,
      v3<Float> const &view,
      v3<Float> const &up,
      v3<Float> const &right)
  {
    auto constexpr PI = std::numbers::pi_v<Float>;

    auto const angle = 2 * PI * rand_unit_();
    auto const radius = aperture_radius_ * std::sqrt(rand_unit_());

    auto const deviation = radius * (std::cos(angle) * right + std::sin(angle) * up);

    auto const origin = position + deviation;
    auto const target = view + dh * right + dv * up;
    auto const target_at_focus_plane = focal_distance_ * target;

    auto const dir = normalize((target_at_focus_plane - deviation));

    return {origin, dir};
  }
};

} // namespace htracer::raytracing::lenses

#endif