#ifndef HTRACER_RENDERING_INTERSECTOR_HPP
#define HTRACER_RENDERING_INTERSECTOR_HPP


#include <htracer/geometries/ray.hpp>
#include <htracer/staging/scene.hpp>

#include <limits>
#include <utility>
#include <vector>


namespace htracer::rendering
{

// TODO: Try returning normals and material etc intead of pointer and see if performance changes.
template<typename Float, typename Scene>
[[nodiscard]]
constexpr auto
intersect(geometries::ray<Float> const &ray, Scene const &scene, Float min_dist)
    -> std::optional<std::pair<Float, staging::object_base<Float> const &>>;


template<typename Float, typename Scene>
constexpr auto
intersect(geometries::ray<Float> const &ray, Scene const &scene, Float min_dist)
    -> std::optional<std::pair<Float, staging::object_base<Float> const &>>
{
  constexpr auto MAX_DISTANCE = std::numeric_limits<Float>::max();

  Float closest_dist = MAX_DISTANCE;
  staging::object_base<Float> const *closest_obj = nullptr;

  scene.for_each_object([min_dist, &ray, &closest_dist, &closest_obj](auto const &obj)
  {
    if (auto dist_o = obj.get_geometry().intersect(ray); dist_o)
    {
      if (auto dist = *dist_o; dist < closest_dist && dist > min_dist)
      {
        closest_dist = dist;
        closest_obj = &obj;
      }
    }
  });

  if (closest_dist < MAX_DISTANCE)
  {
    return std::pair<Float, staging::object_base<Float> const &>(closest_dist, *closest_obj);
  }

  return {};
}

} // namespace htracer::rendering

#endif
