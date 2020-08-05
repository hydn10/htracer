#ifndef HTRACER_RAYTRACING_INTERSECTOR_HPP
#define HTRACER_RAYTRACING_INTERSECTOR_HPP


#include <htracer/geometry/ray.hpp>
#include <htracer/geometry/sphere.hpp>

#include <utility>
#include <vector>


namespace htracer::raytracing
{
template<typename T>
auto
intersect(
    geometry::ray<T> ray,
    const std::vector<geometry::sphere<T>>& objects,
    T min_dist)
    -> std::optional<
        std::pair<T, typename std::vector<geometry::sphere<T>>::const_iterator>>
{
  std::optional<T> closest_dist = std::nullopt;
  typename std::vector<geometry::sphere<T>>::const_iterator closest_obj;

  for (auto it = std::begin(objects); it != std::end(objects); ++it)
  {
    auto dist = it->intersect(ray);

    if (dist && *dist > min_dist && (!closest_dist || *dist < *closest_dist))
    {
      closest_dist = *dist;
      closest_obj = it;
    }
  }

  if (!closest_dist)
    return {};

  return std::make_pair(*closest_dist, closest_obj);
}

} // namespace htracer::raytracer

#endif // HTRACER_RAYTRACING_INTERSECTOR_HPP
