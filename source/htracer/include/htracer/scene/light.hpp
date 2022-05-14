#ifndef HTRACER_SCENE_LIGHT_HPP
#define HTRACER_SCENE_LIGHT_HPP


#include <htracer/colors/colors.hpp>
#include <htracer/vector.hpp>


namespace htracer::scene
{

template<typename Float>
struct light
{
  v3<Float> position;
  colors::srgb_linear<Float> color;
  Float intensity;
};

} // namespace htracer::scene

#endif