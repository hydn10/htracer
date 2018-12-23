#ifndef HDN_RAYTRACER_SAMPLER_HPP
#define HDN_RAYTRACER_SAMPLER_HPP


#include <raytracer/vector.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/color.hpp>
#include <raytracer/intersector.hpp>

#include <vector>


namespace hdn
{

template <typename T>
hdn::color<T>
sample(hdn::ray<T> ray, const std::vector<hdn::sphere<T>> &objects)
{
  hdn::color<T> s = {0, 0, 0};

  auto intersection = intersect(ray, objects);

  if (!intersection)
    return s;
  
  return intersection->second->sample(ray.origin + intersection->first * ray.direction);
}

}

#endif // HDN_RAYTRACER_SAMPLER_HPP