#ifndef HTRACER_SCENE_SCENE_HPP
#define HTRACER_SCENE_SCENE_HPP


#include <htracer/geometry/sphere.hpp>
#include <htracer/scene/light.hpp>

#include <vector>


namespace htracer::scene
{
template<typename T>
struct scene
{
  std::vector<geometry::sphere<T>> objects;
  std::vector<light<T>> lights;
};

} // namespace htracer::scene

#endif // HTRACER_SCENE_SCENE_HPP
