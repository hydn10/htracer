#ifndef HTRACER_GEOMETRY_RAY_HPP
#define HTRACER_GEOMETRY_RAY_HPP


#include <htracer/vector.hpp>


namespace htracer::geometry
{
template<typename Float>
struct ray
{
  v3<Float> origin;
  v3<Float> direction;
};

} // namespace htracer::geometry

#endif // HTRACER_GEOMETRY_RAY_HPP
