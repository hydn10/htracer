#ifndef HDN_RAYTRACER_RAY_HPP
#define HDN_RAYTRACER_RAY_HPP

  
#include <raytracer/vector.hpp>


namespace hdn
{

template <typename T>
struct ray
{
  v3<T> origin;
  v3<T> direction;
};

}

#endif // HDN_RAYTRACER_RAY_HPP