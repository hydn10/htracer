#ifndef HTRACER_RAYTRACING_SAMPLER_HPP
#define HTRACER_RAYTRACING_SAMPLER_HPP


#include <htracer/color.hpp>
#include <htracer/geometry/ray.hpp>
#include <htracer/raytracing/intersector.hpp>
#include <htracer/scene/scene.hpp>
#include <htracer/vector.hpp>


namespace htracer::raytracing
{
template<typename T>
v3<T>
sample(geometry::ray<T> ray, const scene::scene<T>& scene)
{
  auto s = color<T>{{0, 0, 0}};

  auto intersection = intersect(ray, scene.objects, .02);

  if (!intersection)
    return s;

  auto obj = intersection->second;
  auto obj_dist = intersection->first;

  auto light = scene.lights[0];

  auto p = ray.origin + obj_dist * ray.direction;
  auto n = obj->normal(p);

  auto l = light.position - p;

  auto light_dist2_inv = 1 / dot(l, l);

  l = l.normalized();

  auto lambertian = std::max(dot(l, n), T{0});
  T specular = 0;

  if (lambertian > 0)
  {
    auto h = (l - ray.direction).normalized();
    auto spec_angle = std::max(dot(h, n), T{0});
    specular = std::pow(spec_angle, obj->material.shininess);
  }

  auto ambient_color = obj->material.ambient_color;
  auto diffuse_color = obj->material.diffuse_color * lambertian
                       * light.intensity * light_dist2_inv;
  auto specular_color = obj->material.specular_color * specular
                        * light.intensity * light_dist2_inv;

  auto pixel_color = ambient_color + diffuse_color + specular_color;


  if (pixel_color[0] > 255)
    pixel_color[0] = 255;

  if (pixel_color[1] > 255)
    pixel_color[1] = 255;

  if (pixel_color[2] > 255)
    pixel_color[2] = 255;

  // pixel_color[0] = 255 * std::pow(pixel_color[0] / T{255}, 2.2);
  // pixel_color[1] = 255 * std::pow(pixel_color[1] / T{255}, 1/2.2);
  // pixel_color[2] = 255 * std::pow(pixel_color[2] / T{255}, 1/2.2);

  return pixel_color;
}

} // namespace htracer::raytracing

#endif // HTRACER_RAYTRACING_SAMPLER_HPP
