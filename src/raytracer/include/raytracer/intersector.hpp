#ifndef HDN_RAYTRACER_INTERSECTOR_HPP
#define HDN_RAYTRACER_INTERSECTOR_HPP


#include <raytracer/ray.hpp>
#include <raytracer/sphere.hpp>

#include <utility>
#include <vector>


namespace hdn
{
template<typename T>
auto
intersect(
      hdn::ray<T> ray,
      const std::vector<hdn::sphere<T>>& objects,
      T min_dist)
      -> std::optional<
            std::pair<T, typename std::vector<hdn::sphere<T>>::const_iterator>>
{
  std::optional<T> closest_dist = std::nullopt;
  typename std::vector<hdn::sphere<T>>::const_iterator closes_obj;

  for (auto it = std::begin(objects); it != std::end(objects); ++it)
  {
    auto dist = it->intersect(ray);

    if (dist && *dist > min_dist && (!closest_dist || *dist < *closest_dist))
    {
      closest_dist = *dist;
      closes_obj = it;
    }
  }

  if (!closest_dist)
    return {};

  return std::make_pair(*closest_dist, closes_obj);
}

} // namespace hdn

#endif // HDN_RAYTRACER_INTERSECTOR_HPP
