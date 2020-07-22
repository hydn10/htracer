#ifndef HDN_RAYTRACER_SCENE_HPP
#define HDN_RAYTRACER_SCENE_HPP


#include <raytracer/sphere.hpp>
#include <raytracer/light.hpp>

#include <vector>


namespace hdn
{

template <typename T>
struct scene
{
  std::vector<hdn::sphere<T>> objects;
  std::vector<hdn::light<T>> lights;
};

} // namespace hdn

#endif // HDN_RAYTRACER_SCENE_HPP
