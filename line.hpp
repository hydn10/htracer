#ifndef HDN_RAYTRACER_LINE_HPP
#define HDN_RAYTRACER_LINE_HPP



#include "vector.hpp"


namespace hdn
{

template <typename T>
struct line
{
  v3<T> origin;
  v3<T> direction;
};

}

#endif // HDN_RAYTRACER_LINE_HPP
