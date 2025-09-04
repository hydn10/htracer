#ifndef HTRACER_FLOAT_TRAITS_HPP
#define HTRACER_FLOAT_TRAITS_HPP


#include <htracer/htracer.hpp>


namespace htracer
{

template<std::floating_point Float>
struct float_traits
{
  using float_type = Float;

  using v3 = htracer::v3<Float>;

  using srgb = htracer::colors::srgb<Float>;
  using srgb_linear = htracer::colors::srgb_linear<Float>;
  using hsl = htracer::colors::hsl<Float>;

  using ray = htracer::geometries::ray<Float>;
  using sphere = htracer::geometries::sphere<Float>;
  using plane = htracer::geometries::plane<Float>;

  using material = htracer::staging::material<Float>;
  using light = htracer::staging::light<Float>;
  using scene = htracer::staging::scene<Float>;

  [[nodiscard]]
  static constexpr auto
  make_solid_material(srgb_linear const &color, Float ambient, Float specular, Float shininess, Float reflectivity)
  {
    return htracer::staging::make_solid<Float>(color, ambient, specular, shininess, reflectivity);
  }

  [[nodiscard]]
  static constexpr auto
  make_mirror_material(Float specular, Float shininess, Float reflectivity)
  {
    return htracer::staging::make_mirror<Float>(specular, shininess, reflectivity);
  }

  using camera = htracer::rendering::camera<Float>;
  using image = htracer::rendering::image<Float>;
  using pinhole_lens = htracer::rendering::lenses::pinhole_lens<Float>;
  using thin_lens = htracer::rendering::lenses::thin_lens<Float>;
  using point_sensor = htracer::rendering::sensors::point_sensor<Float>;
  using uniform_sensor = htracer::rendering::sensors::uniform_sensor<Float>;
};

} // namespace htracer

#endif
