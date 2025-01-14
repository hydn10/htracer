#ifndef HTRACER_GEOMETRIES_GEOMETRY_HPP
#define HTRACER_GEOMETRIES_GEOMETRY_HPP


#include <htracer/geometries/ray.hpp>

#include <utility>


namespace htracer::geometries
{

template<typename Float>
class geometry
{
public:
  geometry() = default;

  geometry(geometry<Float> const &) = default;
  geometry(geometry<Float> &&) = default;
  geometry<Float> &
  operator=(geometry<Float> const &) = default;
  geometry<Float> &
  operator=(geometry<Float> &&) = default;

  virtual ~geometry() = default;

  // TODO: nodiscard? how does it work with inheritance?
  virtual v3<Float>
  normal(v3<Float> const &point) const = 0;
};

} // namespace htracer::geometries

#endif
