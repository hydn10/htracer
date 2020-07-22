#ifndef HDN_RAYTRACER_MATERIAL_HPP
#define HDN_RAYTRACER_MATERIAL_HPP


#include <raytracer/vector.hpp>


namespace hdn
{

template <typename T>
struct material
{
  v3<T> ambient_color;
  v3<T> diffuse_color;
  v3<T> specular_color;
  T shininess;
};

} // namespace hdn

#endif // HDN_RAYTRACER_MATERIAL_HPP
