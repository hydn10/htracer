#ifndef HTRACER_GEOMETRIES_RAY_HPP
#define HTRACER_GEOMETRIES_RAY_HPP


#include <htracer/vector.hpp>


namespace htracer::geometries
{
template<typename Float>
struct ray
{
  v3<Float> origin;
  v3<Float> direction;
};

} // namespace htracer::geometries

#endif