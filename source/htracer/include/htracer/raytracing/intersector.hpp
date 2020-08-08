#ifndef HTRACER_RAYTRACING_INTERSECTOR_HPP
#define HTRACER_RAYTRACING_INTERSECTOR_HPP


#include <htracer/geometry/ray.hpp>
#include <htracer/scene/scene.hpp>

#include <utility>
#include <vector>


namespace htracer::raytracing
{
template<typename Float>
auto
intersect(
    geometry::ray<Float> ray,
    const typename scene::scene<Float>::container& objects,
    Float min_dist)
    -> std::optional<std::pair<
        Float,
        typename scene::scene<Float>::container::const_iterator>>
{
  std::optional<Float> closest_dist = std::nullopt;
  auto closest_obj = objects.cend();

  for (auto it = objects.cbegin(); it != objects.cend(); ++it)
  {
    auto dist = std::visit(
        [&ray](const auto& v) { return v.get().geometry.intersect(ray); }, *it);

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
