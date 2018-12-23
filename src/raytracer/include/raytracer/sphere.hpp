#ifndef HDN_RAYTRACER_SPHERE_HPP
#define HDN_RAYTRACER_SPHERE_HPP


#include <raytracer/vector.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/color.hpp>

#include <optional>


namespace hdn
{

template <typename T>
class sphere
{
  v3<T> center_;
  T radius_;

public:
  sphere(v3<T> center, T radius);
  ~sphere() = default;

  const v3<T>&
  center() const;

  T
  radius() const;

  std::optional<T>
  intersect(const ray<T> &line) const;

  v3<T>
  normal(const v3<T> &point) const;

  color<T>
  sample(const v3<T> &point) const;
};



template <typename T>
sphere<T>::sphere(v3<T> center, T radius)
  :
    center_{center},
    radius_{radius}
{

}


template <typename T>
const v3<T>&
sphere<T>::center() const
{
  return center_;
}


template <typename T>
T
sphere<T>::radius() const
{
  return radius_;
}


template <typename T>
std::optional<T>
sphere<T>::intersect(const ray<T> &ray) const
{
  auto oc = center_ - ray.origin;

  auto sp_voc = dot(ray.direction, oc);
  auto sp_ococ = dot(oc, oc);

  auto disc = sp_voc*sp_voc - sp_ococ + radius_*radius_;

  if (disc <= 0)
    return {};
  
  return sp_voc - std::sqrt(disc);
}


template <typename T>
v3<T>
sphere<T>::normal(const v3<T> &point) const
{
  return (point - center_).normalized();
}


template <typename T>
color<T>
sphere<T>::sample(const v3<T> &point) const
{
  return {240, 0, 0};
}

}

#endif // HDN_RAYTRACER_SPHERE_HPP
