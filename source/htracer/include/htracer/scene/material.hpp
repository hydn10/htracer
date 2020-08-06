#ifndef HTRACER_SCENE_MATERIAL_HPP
#define HTRACER_SCENE_MATERIAL_HPP


#include <htracer/color.hpp>


namespace htracer::scene
{
template<typename Float>
struct material
{
  color<Float> ambient_color;
  color<Float> diffuse_color;
  color<Float> specular_color;
  Float shininess;
};

} // namespace htracer::scene

#endif // HTRACER_SCENE_MATERIAL_HPP
