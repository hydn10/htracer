#ifndef HTRACER_FLOAT_TRAITS_HPP
#define HTRACER_FLOAT_TRAITS_HPP


#include <htracer/rendering/batchers/column_batcher.hpp>
#include <htracer/rendering/batchers/pixel_batcher.hpp>
#include <htracer/rendering/lenses/pinhole_lens.hpp>
#include <htracer/rendering/lenses/thin_lens.hpp>
#include <htracer/rendering/renderers/deterministic_renderer.hpp>
#include <htracer/rendering/renderers/randomized_renderer.hpp>
#include <htracer/rendering/samplers/deterministic_sampler.hpp>
#include <htracer/rendering/samplers/repeat_sampler.hpp>
#include <htracer/rendering/sensors/point_sensor.hpp>
#include <htracer/rendering/sensors/uniform_sensor.hpp>
#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>
#include <htracer/rendering/image.hpp>
#include <htracer/rendering/policies.hpp>
#include <htracer/rendering/rendering.hpp>

#include <htracer/staging/light.hpp>
#include <htracer/staging/material.hpp>
#include <htracer/staging/object.hpp>
#include <htracer/staging/object_base.hpp>
#include <htracer/staging/scene.hpp>
#include <htracer/staging/scene_with.hpp>

#include <htracer/vector.hpp>


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
