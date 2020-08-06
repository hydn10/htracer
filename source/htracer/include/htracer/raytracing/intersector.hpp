#ifndef HTRACER_RAYTRACING_INTERSECTOR_HPP
#define HTRACER_RAYTRACING_INTERSECTOR_HPP


#include <htracer/geometry/ray.hpp>
#include <htracer/geometry/sphere.hpp>
#include <htracer/scene/object.hpp>

#include <utility>
#include <vector>


namespace htracer::raytracing
{
template<typename Float>
auto
intersect(
    geometry::ray<Float> ray,
    const std::vector<scene::object<Float, geometry::sphere>>& objects,
    Float min_dist)
    -> std::optional<std::pair<
        Float,
        typename std::vector<
            scene::object<Float, geometry::sphere>>::const_iterator>>
{
  std::optional<Float> closest_dist = std::nullopt;
  typename std::vector<scene::object<Float, geometry::sphere>>::const_iterator
      closest_obj;

  for (auto it = std::cbegin(objects); it != cend(objects); ++it)
  {
    auto dist = it->geometry.intersect(ray);

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

} // namespace htracer::raytracing

#endif // HTRACER_RAYTRACING_INTERSECTOR_HPP
