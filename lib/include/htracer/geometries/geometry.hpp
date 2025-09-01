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
  geometry() noexcept = default;

  geometry(geometry<Float> const &) noexcept = default;
  geometry(geometry<Float> &&) noexcept = default;
  geometry<Float> &
  operator=(geometry<Float> const &) noexcept = default;
  geometry<Float> &
  operator=(geometry<Float> &&) noexcept = default;

  virtual ~geometry() = default;

  [[nodiscard]]
  virtual v3<Float>
  normal(v3<Float> const &point) const noexcept = 0;
};

} // namespace htracer::geometries

#endif