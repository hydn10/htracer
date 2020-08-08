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
  auto intersection = intersect(ray, scene.objects(), .02);

  if (!intersection)
    return {0., 0., 0.};

  auto obj_dist = intersection->first;
  auto obj_iter = intersection->second;

  auto const& lights = scene.lights();
  auto const& light = lights[0];

  auto p = ray.origin + obj_dist * ray.direction;
  auto n = std::visit(
      [&p](const auto& v) { return v.get().geometry.normal(p); }, *obj_iter);

  auto l = light.position - p;

  auto light_dist2_inv = 1 / dot(l, l);

  l = l.normalized();

  auto lambertian = std::max(dot(l, n), Float{0});
  Float specular = 0;

  const auto& material = std::visit(
      [](const auto& v) -> scene::material<Float> const& {
        return v.get().mat;
      },
      *obj_iter);

  if (lambertian > 0)
  {
    auto h = (l - ray.direction).normalized();
    auto spec_angle = std::max(dot(h, n), Float{0});
    specular = std::pow(spec_angle, material.shininess);
  }

  auto ambient_color = material.ambient_color;
  auto diffuse_color =
      material.diffuse_color * lambertian * light.intensity * light_dist2_inv;
  auto specular_color =
      material.specular_color * specular * light.intensity * light_dist2_inv;

  auto pixel_color = ambient_color + diffuse_color + specular_color;

  // After reading a lot about gamma, I still don't know if I am supposed to
  // gamma correct the output or not.
  // Probably experiment more and see what I like the best.
  // Take in accout that this will change color values if they are hardcoded.
  // They must be raised to the 1/2.2 power (or viceversa?) if they are to be
  // compared.
  pixel_color = pow(pixel_color, 1 / 2.2);

  return saturate(pixel_color);
}

} // namespace htracer::raytracing

#endif // HTRACER_RAYTRACING_SAMPLER_HPP
