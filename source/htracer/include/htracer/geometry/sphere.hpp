#ifndef HTRACER_GEOMETRY_SPHERE_HPP
#define HTRACER_GEOMETRY_SPHERE_HPP


#include <htracer/color.hpp>
#include <htracer/geometry/ray.hpp>
#include <htracer/scene/material.hpp>
#include <htracer/vector.hpp>

#include <optional>
#include <utility>


namespace htracer::geometry
{
template<typename T>
class sphere
{
  v3<T> center_;
  T radius_;

public:
  scene::material<T> material;

  sphere(v3<T> center, T radius, scene::material<T> material);
  ~sphere() = default;

  [[nodiscard]] const v3<T>&
  center() const;

  [[nodiscard]] T
  radius() const;

  std::optional<T>
  intersect(const ray<T>& line) const;

  v3<T>
  normal(const v3<T>& point) const;
};


template<typename T>
sphere<T>::sphere(v3<T> center, T radius, scene::material<T> material)
    : center_{center}, radius_{radius}, material{std::move(material)}
{
}


template<typename T>
const v3<T>&
sphere<T>::center() const
{
  return center_;
}


template<typename T>
T
sphere<T>::radius() const
{
  return radius_;
}


template<typename T>
std::optional<T>
sphere<T>::intersect(const ray<T>& ray) const
{
  auto oc = center_ - ray.origin;

  auto dot_voc = dot(ray.direction, oc);
  auto dot_ococ = dot(oc, oc);

  auto disc = dot_voc * dot_voc - dot_ococ + radius_ * radius_;

  if (disc <= 0)
    return {};

  return dot_voc - std::sqrt(disc);
}


template<typename T>
v3<T>
sphere<T>::normal(const v3<T>& point) const
{
  return (point - center_).normalized();
}

} // namespace htracer::geometry

#endif // HTRACER_GEOMETRY_SPHERE_HPP
