#ifndef HTRACER_RENDERING_LENSES_THIN_LENS_HPP
#define HTRACER_RENDERING_LENSES_THIN_LENS_HPP


#include <htracer/geometries/ray.hpp>
#include <htracer/vector.hpp>

#include <numbers>
#include <random>


namespace htracer::rendering::lenses
{

template<typename Float>
class thin_lens
{
  Float aperture_radius_;
  Float focal_distance_;

public:
  thin_lens(Float aperture_radius, Float focal_distance) noexcept;

  template<std::uniform_random_bit_generator Generator>
  [[nodiscard]]
  geometries::ray<Float>
  get_ray(
      Float dv,
      Float dh,
      v3<Float> const &position,
      v3<Float> const &view,
      v3<Float> const &up,
      v3<Float> const &right,
      Generator &generator) const;
};


template<typename Float>
thin_lens<Float>::thin_lens(Float aperture_radius, Float focal_distance) noexcept
    : aperture_radius_{aperture_radius}
    , focal_distance_{focal_distance}
{
}


template<typename Float>
template<std::uniform_random_bit_generator Generator>
geometries::ray<Float>
thin_lens<Float>::get_ray(
    Float dv,
    Float dh,
    v3<Float> const &position,
    v3<Float> const &view,
    v3<Float> const &up,
    v3<Float> const &right,
    Generator &generator) const
{
  auto constexpr PI = std::numbers::pi_v<Float>;

  std::uniform_real_distribution<Float> zero_one_dist{0, 1};

  auto const random_unit = [&zero_one_dist, &generator]()
  {
    return zero_one_dist(generator);
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

} // namespace htracer::rendering::lenses

#endif
