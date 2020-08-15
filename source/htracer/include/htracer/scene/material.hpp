#ifndef HTRACER_SCENE_MATERIAL_HPP
#define HTRACER_SCENE_MATERIAL_HPP


#include <htracer/colors/colors.hpp>


namespace htracer::scene
{
template<typename Float>
struct material
{
  colors::srgb_linear<Float> ambient_color;
  colors::srgb_linear<Float> diffuse_color;
  Float specular;
  Float shininess;
  Float reflectivity;
};

} // namespace htracer::scene

#endif // HTRACER_SCENE_MATERIAL_HPP
