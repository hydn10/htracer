#ifndef HTRACER_GEOMETRY_PLANE_HPP
#define HTRACER_GEOMETRY_PLANE_HPP


#include <htracer/geometry/ray.hpp>
#include <htracer/vector.hpp>

#include <optional>
#include <utility>


namespace htracer::geometry
{
template<typename Float>
class plane
{
  const v3<Float> anchor_;
  const v3<Float> up_;

public:
  plane(const v3<Float>& anchor, const v3<Float>& up);

  [[nodiscard]] const v3<Float>&
  anchor() const;

  [[nodiscard]] const v3<Float>&
  up() const;

  std::optional<Float>
  intersect(const ray<Float>& ray) const;

  v3<Float>
  normal(const v3<Float>& point) const;
};


template<typename Float>
plane<Float>::plane(const v3<Float>& anchor, const v3<Float>& up)
    : anchor_{anchor}, up_{up.normalized()}
{
}


template<typename Float>
const v3<Float>&
plane<Float>::anchor() const
{
  return anchor_;
}


template<typename Float>
const v3<Float>&
plane<Float>::up() const
{
  return up_;
}


template<typename Float>
std::optional<Float>
plane<Float>::intersect(const ray<Float>& ray) const
{
  auto dot_dn = dot(ray.direction, up_);

  if (dot_dn == Float{0})
    return {};

  auto dot_aon = dot(anchor_ - ray.origin, up_);
  return dot_aon / dot_dn;
}


template<typename Float>
v3<Float>
plane<Float>::normal(const v3<Float>&) const
{
  return up_;
}

} // namespace htracer::geometry

#endif // HTRACER_GEOMETRY_PLANE_HPP
