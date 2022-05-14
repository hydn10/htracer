#ifndef HTRACER_RAYTRACING_INTERSECTOR_HPP
#define HTRACER_RAYTRACING_INTERSECTOR_HPP


#include <htracer/geometries/ray.hpp>
#include <htracer/scene/scene_view.hpp>

#include <limits>
#include <utility>
#include <vector>


namespace htracer::raytracing
{

// TODO: Try returning normals and material etc intead of pointer and see if performance changes.
template<typename Float, template<typename> typename... Geometries>
auto
intersect(
    const geometries::ray<Float> &ray, scene::scene_view<scene::scene<Float, Geometries...>> scene, Float min_dist)
    -> std::optional<std::pair<Float, scene::object_base<Float> const &>>
{
  constexpr auto MAX_DISTANCE = std::numeric_limits<Float>::max();

  // Check performance diference with std::optional;
  Float closest_dist = MAX_DISTANCE;
  scene::object_base<Float> const *closest_obj;

  scene.for_each_object(
      [min_dist, &ray, &closest_dist, &closest_obj](auto const &obj)
      {
        if (auto dist = obj.geometry().intersect(ray); dist && dist.value() < closest_dist && dist.value() > min_dist)
        {
          closest_dist = dist.value();
          closest_obj = &obj;
        }
      });

  if (closest_dist < MAX_DISTANCE)
    return std::pair<Float, scene::object_base<Float> const &>(closest_dist, *closest_obj);

  return {};
}

} // namespace htracer::raytracing

#endif