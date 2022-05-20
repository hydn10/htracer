#ifndef HTRACER_GEOMETRY_PLANE_HPP
#define HTRACER_GEOMETRY_PLANE_HPP


#include <htracer/geometries/geometry.hpp>
#include <htracer/geometries/ray.hpp>
#include <htracer/vector.hpp>

#include <optional>


namespace htracer::geometries
{

template<typename Float>
class plane final : public geometry<Float>
{
  v3<Float> anchor_;
  v3<Float> up_;

public:
  plane(const v3<Float> &anchor, v3<Float> const &up);

  [[nodiscard]] const v3<Float> &
  anchor() const;

  [[nodiscard]] const v3<Float> &
  up() const;

  // TODO: nodiscard?
  // TODO: Return 0 as distance if no intersection and check performance?
  //       Should be careful as in theory it can return negative values.
  std::optional<Float>
  intersect(ray<Float> const &ray) const;

  // TODO: nodiscard? how does it work with inheritance?
  v3<Float>
  normal(v3<Float> const &point) const override;
};


template<typename Float>
plane<Float>::plane(v3<Float> const &anchor, v3<Float> const &up)
    : anchor_{anchor}
    , up_{normalize(up)}
{
}


template<typename Float>
const v3<Float> &
plane<Float>::anchor() const
{
  return anchor_;
}


template<typename Float>
const v3<Float> &
plane<Float>::up() const
{
  return up_;
}


template<typename Float>
std::optional<Float>
plane<Float>::intersect(ray<Float> const &ray) const
{
  auto dot_dn = dot(ray.direction, up_);

  if (dot_dn == Float{0})
    return {};

  auto dot_aon = dot(anchor_ - ray.origin, up_);
  return dot_aon / dot_dn;
}


template<typename Float>
v3<Float>
plane<Float>::normal(v3<Float> const &) const
{
  return up_;
}

} // namespace htracer::geometries

#endif