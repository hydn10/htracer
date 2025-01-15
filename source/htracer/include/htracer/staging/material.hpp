#ifndef HTRACER_STAGING_MATERIAL_HPP
#define HTRACER_STAGING_MATERIAL_HPP


#include <htracer/colors/colors.hpp>


namespace htracer::staging
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


template<typename Float>
constexpr material<Float>
make_solid(colors::srgb_linear<Float> const &color, Float ambient, Float specular, Float shininess, Float reflectivity);


template<typename Float>
constexpr material<Float>
make_mirror(Float specular, Float shininess, Float reflectivity);


template<typename Float>
constexpr material<Float>
make_solid(colors::srgb_linear<Float> const &color, Float ambient, Float specular, Float shininess, Float reflectivity)
{
  return {ambient * color, color, specular, shininess, reflectivity};
}


template<typename Float>
constexpr material<Float>
make_mirror(Float specular, Float shininess, Float reflectivity)
{
  // TODO: Careful! Even if ambient and diffuse are 0, specular color is still
  //       relevant. It should be added to inputs once I handle it correctly on
  //       sample().
  return {{0., 0., 0.}, {0., 0., 0.}, specular, shininess, reflectivity};
}

} // namespace htracer::staging

#endif
