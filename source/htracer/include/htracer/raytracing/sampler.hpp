#ifndef HTRACER_RAYTRACING_SAMPLER_HPP
#define HTRACER_RAYTRACING_SAMPLER_HPP


#include <htracer/color.hpp>
#include <htracer/geometry/ray.hpp>
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
    return color<Float>{{0, 0, 0}};

  auto obj_dist = intersection->first;
  auto obj_iter = intersection->second;

  auto const& lights = scene.lights();
  auto const& light = lights[0];

  auto p = ray.origin + obj_dist * ray.direction;
  auto n = obj_iter->geometry.normal(p);

  auto l = light.position - p;

  auto light_dist2_inv = 1 / dot(l, l);

  l = l.normalized();

  auto lambertian = std::max(dot(l, n), Float{0});
  Float specular = 0;

  if (lambertian > 0)
  {
    auto h = (l - ray.direction).normalized();
    auto spec_angle = std::max(dot(h, n), Float{0});
    specular = std::pow(spec_angle, obj_iter->mat.shininess);
  }

  auto ambient_color = obj_iter->mat.ambient_color;
  auto diffuse_color = obj_iter->mat.diffuse_color * lambertian
                       * light.intensity * light_dist2_inv;
  auto specular_color = obj_iter->mat.specular_color * specular
                        * light.intensity * light_dist2_inv;

  auto pixel_color = ambient_color + diffuse_color + specular_color;


  if (pixel_color[0] > 255)
    pixel_color[0] = 255;

  if (pixel_color[1] > 255)
    pixel_color[1] = 255;

  if (pixel_color[2] > 255)
    pixel_color[2] = 255;

  // pixel_color[0] = 255 * std::pow(pixel_color[0] / Float{255}, 2.2);
  // pixel_color[1] = 255 * std::pow(pixel_color[1] / Float{255}, 1/2.2);
  // pixel_color[2] = 255 * std::pow(pixel_color[2] / Float{255}, 1/2.2);

  return pixel_color;
}

} // namespace htracer::raytracing

#endif // HTRACER_RAYTRACING_SAMPLER_HPP
