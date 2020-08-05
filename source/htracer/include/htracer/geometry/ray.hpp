#ifndef HTRACER_GEOMETRY_RAY_HPP
#define HTRACER_GEOMETRY_RAY_HPP


#include <htracer/vector.hpp>


namespace htracer::geometry
{
template<typename T>
struct ray
{
  v3<T> origin;
  v3<T> direction;
};

} // namespace htracer::geometry

#endif // HTRACER_GEOMETRY_RAY_HPP
