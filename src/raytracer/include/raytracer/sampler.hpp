#ifndef HDN_RAYTRACER_SAMPLER_HPP
#define HDN_RAYTRACER_SAMPLER_HPP


#include <raytracer/vector.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/color.hpp>
#include <raytracer/intersector.hpp>
#include <raytracer/scene.hpp>


namespace hdn
{

template <typename T>
hdn::v3<T>
sample(hdn::ray<T> ray, const hdn::scene<T> &scene)
{
  hdn::color<T> s = {{0, 0, 0}};

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

  auto pixel_color =
      obj->material.ambient_color +
      obj->material.diffuse_color * lambertian * light.intensity * light_dist2_inv + 
      obj->material.specular_color * specular * light.intensity * light_dist2_inv;
  
  if (pixel_color[0] > 255)
    pixel_color[0] = 255;
  if (pixel_color[1] > 255)
    pixel_color[1] = 255;
  if (pixel_color[2] > 255)
    pixel_color[2] = 255;

  //pixel_color[0] = 255 * std::pow(pixel_color[0] / T{255}, 2.2);
  //pixel_color[1] = 255 * std::pow(pixel_color[1] / T{255}, 1/2.2);
  //pixel_color[2] = 255 * std::pow(pixel_color[2] / T{255}, 1/2.2);

  return pixel_color;
}

} // namespace std

#endif // HDN_RAYTRACER_SAMPLER_HPP
