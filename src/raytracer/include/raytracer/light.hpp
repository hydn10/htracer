#ifndef HDN_RAYTRACER_LIGHT_HPP
#define HDN_RAYTRACER_LIGHT_HPP


#include <raytracer/light.hpp>


namespace hdn
{

template <typename T>
struct light
{
  v3<T> position;
  v3<T> color;
  T intensity;
};

}

#endif // HDN_RAYTRACER_LIGHT_HPP