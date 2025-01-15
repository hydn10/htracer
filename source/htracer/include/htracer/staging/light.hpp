#ifndef HTRACER_STAGING_LIGHT_HPP
#define HTRACER_STAGING_LIGHT_HPP


#include <htracer/colors/colors.hpp>
#include <htracer/vector.hpp>


namespace htracer::staging
{

template<typename Float>
struct light
{
  v3<Float> position;
  colors::srgb_linear<Float> color;
  Float intensity;
};

} // namespace htracer::staging

#endif
