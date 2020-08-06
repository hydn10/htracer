#ifndef HTRACER_SCENE_LIGHT_HPP
#define HTRACER_SCENE_LIGHT_HPP


#include <htracer/vector.hpp>


namespace htracer::scene
{
template<typename Float>
struct light
{
  v3<Float> position;
  v3<Float> color;
  Float intensity;
};

} // namespace htracer::scene

#endif // HTRACER_SCENE_LIGHT_HPP
