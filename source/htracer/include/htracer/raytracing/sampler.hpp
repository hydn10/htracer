#ifndef HTRACER_RAYTRACING_SAMPLER_HPP
#define HTRACER_RAYTRACING_SAMPLER_HPP


#include <htracer/color.hpp>
#include <htracer/geometry/ray.hpp>
#include <htracer/numerics.hpp>
#include <htracer/raytracing/intersector.hpp>
#include <htracer/scene/scene.hpp>
#include <htracer/vector.hpp>


namespace htracer::raytracing
{
template<typename Float>
v3<Float>
sample(geometry::ray<Float> ray, const scene::scene<Float>& scene)
{
  // TODO: This should be a parameter since it is scale-dependant
  constexpr Float MIN_DISTANCE = .0;

  const auto intersection = intersect(ray, scene.objects(), MIN_DISTANCE);

  if (!intersection)
    return {0., 0., 0.};

  auto const& [obj_dist, obj_iter] = *intersection;

  auto const& material = std::visit(
      [](const auto& v) -> scene::material<Float> const& {
        return v.get().mat;
      },
      *obj_iter);

  auto const& lights = scene.lights();
  auto const& light = lights[0];

  const auto p = ray.origin + obj_dist * ray.direction;
  const auto n = std::visit(
      [&p](const auto& v) { return v.get().geometry.normal(p); }, *obj_iter);

  const auto pl = light.position - p;
  const auto light_dist2_inv = 1 / dot(pl, pl);
  const auto l = pl.normalized();

  const auto lambertian = std::max(dot(l, n), Float{0});
  Float specular = 0;

  if (lambertian > 0)
  {
    const auto h = (l - ray.direction).normalized();
    const auto spec_angle = std::max(dot(h, n), Float{0});
    specular = std::pow(spec_angle, material.shininess);
  }

  const auto ambient_color = material.ambient_color;
  const auto diffuse_color =
      material.diffuse_color * lambertian * light.intensity * light_dist2_inv;
  const auto specular_color =
      material.specular_color * specular * light.intensity * light_dist2_inv;

  const auto pixel_color = ambient_color + diffuse_color + specular_color;

  // After reading a lot about gamma, I still don't know if I am supposed to
  // gamma correct the output or not.
  // Probably experiment more and see what I like the best.
  // Take in account that this will change input color values if they are
  // hardcoded. They must be raised to the 1/2.2 power (or viceversa?) if they
  // are to be compared.
  const auto saturated = saturate(pixel_color);
  const auto corrected_pixel_color = pow(saturated, 1 / 2.2);

  return saturate(corrected_pixel_color);
}

} // namespace htracer::raytracing

#endif // HTRACER_RAYTRACING_SAMPLER_HPP
