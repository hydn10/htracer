#ifndef HTRACER_RAYTRACING_LENSES_THIN_HPP
#define HTRACER_RAYTRACING_LENSES_THIN_HPP


#include <htracer/geometries/ray.hpp>
#include <htracer/utils/randomness.hpp>
#include <htracer/vector.hpp>

#include <numbers>
#include <random>


namespace htracer::raytracing::lenses
{

template<typename Float>
class thin
{
  Float aperture_radius_;
  Float focal_distance_;
  std::uniform_real_distribution<Float> zero_one_dist_{0, 1};

public:
  thin(Float aperture_radius, Float focal_distance)
      : aperture_radius_{aperture_radius}
      , focal_distance_{focal_distance}
  {
  }

  template<typename Engine>
  geometries::ray<Float>
  get_ray(
      Float dv,
      Float dh,
      v3<Float> const &position,
      v3<Float> const &view,
      v3<Float> const &up,
      v3<Float> const &right,
      htracer::utils::randomness<Engine> &rand);
};


template<typename Float>
template<typename Engine>
geometries::ray<Float>
thin<Float>::get_ray(
    Float dv,
    Float dh,
    v3<Float> const &position,
    v3<Float> const &view,
    v3<Float> const &up,
    v3<Float> const &right,
    htracer::utils::randomness<Engine> &rand)
{
  auto constexpr PI = std::numbers::pi_v<Float>;

  auto const random_unit = [this, &rand]()
  {
    return rand(zero_one_dist_);
  };

  auto const angle = 2 * PI * random_unit();
  auto const radius = aperture_radius_ * std::sqrt(random_unit());

  auto const deviation = radius * (std::cos(angle) * right + std::sin(angle) * up);

  auto const origin = position + deviation;
  auto const target = view + dh * right + dv * up;
  auto const target_at_focus_plane = focal_distance_ * target;

  auto const dir = normalize((target_at_focus_plane - deviation));

  return {origin, dir};
}

} // namespace htracer::raytracing::lenses

#endif
