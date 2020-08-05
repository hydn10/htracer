#ifndef HTRACER_SCENE_LIGHT_HPP
#define HTRACER_SCENE_LIGHT_HPP


#include <htracer/vector.hpp>


namespace htracer::scene
{
template<typename T>
struct light
{
  v3<T> position;
  v3<T> color;
  T intensity;
};

} // namespace htracer::scene

#endif // HTRACER_SCENE_LIGHT_HPP
