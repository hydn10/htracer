#ifndef HTRACER_GEOMETRY_SPHERE_HPP
#define HTRACER_GEOMETRY_SPHERE_HPP


#include <htracer/geometry/ray.hpp>
#include <htracer/vector.hpp>

#include <optional>
#include <utility>


namespace htracer::geometry
{
template<typename Float>
class sphere
{
  const v3<Float> center_;
  const Float radius_;

public:
  sphere(const v3<Float>& center, Float radius);

  [[nodiscard]] const v3<Float>&
  center() const;

  [[nodiscard]] Float
  radius() const;

  std::optional<Float>
  intersect(const ray<Float>& ray) const;

  v3<Float>
  normal(const v3<Float>& point) const;
};


template<typename Float>
sphere<Float>::sphere(const v3<Float>& center, Float radius)
    : center_{center}, radius_{radius}
{
}


template<typename Float>
const v3<Float>&
sphere<Float>::center() const
{
  return center_;
}


template<typename Float>
Float
sphere<Float>::radius() const
{
  return radius_;
}


template<typename Float>
std::optional<Float>
sphere<Float>::intersect(const ray<Float>& ray) const
{
  auto oc = center_ - ray.origin;

  auto dot_voc = dot(ray.direction, oc);
  auto dot_ococ = dot(oc, oc);

  auto disc = dot_voc * dot_voc - dot_ococ + radius_ * radius_;

  if (disc <= 0)
    return {};

  return dot_voc - std::sqrt(disc);
}


template<typename Float>
v3<Float>
sphere<Float>::normal(const v3<Float>& point) const
{
  return (point - center_).normalized();
}

} // namespace htracer::geometry

#endif // HTRACER_GEOMETRY_SPHERE_HPP
