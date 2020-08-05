#ifndef HTRACER_SCENE_MATERIAL_HPP
#define HTRACER_SCENE_MATERIAL_HPP


#include <htracer/color.hpp>


namespace htracer::scene
{
template<typename T>
struct material
{
  color<T> ambient_color;
  color<T> diffuse_color;
  color<T> specular_color;
  T shininess;
};

} // namespace htracer::scene

#endif // HTRACER_SCENE_MATERIAL_HPP
