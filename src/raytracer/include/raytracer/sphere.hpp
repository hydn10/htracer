#ifndef HDN_RAYTRACER_SPHERE_HPP
#define HDN_RAYTRACER_SPHERE_HPP


#include <raytracer/color.hpp>
#include <raytracer/material.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/vector.hpp>

#include <optional>


namespace hdn
{
template<typename T>
class sphere
{
  v3<T> center_;
  T radius_;

public:
  hdn::material<T> material;

  sphere(v3<T> center, T radius, hdn::material<T> material);
  ~sphere() = default;

  const v3<T>&
  center() const;

  T
  radius() const;

  std::optional<T>
  intersect(const ray<T>& line) const;

  v3<T>
  normal(const v3<T>& point) const;
};


template<typename T>
sphere<T>::sphere(v3<T> center, T radius, hdn::material<T> material)
  : center_{center}, radius_{radius}, material{material}
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

} // namespace hdn

#endif // HDN_RAYTRACER_SPHERE_HPP
