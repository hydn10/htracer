#ifndef HDN_RAYTRACER_SPHERE_HPP
#define HDN_RAYTRACER_SPHERE_HPP



#include "vector.hpp"
#include "line.hpp"

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

  std::optional<v3<T>>
  intersect(const line<T> &line) const;
};



template <typename T>
sphere<T>::sphere(v3<T> center, T radius)
  :
    center_{center},
    radius_{radius_},
{

}


template <typename T>
std::optional<v3<T>>
sphere<T>::intersect(const line<T> &line) const
{

}

}

#endif // HDN_RAYTRACER_SPHERE_HPP
