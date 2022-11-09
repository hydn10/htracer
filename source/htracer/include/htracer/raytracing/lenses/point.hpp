#ifndef HTRACER_RAYTRACING_LENSES_POINT_HPP
#define HTRACER_RAYTRACING_LENSES_POINT_HPP


#include <htracer/geometries/ray.hpp>
#include <htracer/vector.hpp>


namespace htracer::raytracing::lenses
{

template<typename Float>
class point
{
public:
  geometries::ray<Float>
  get_ray(
      Float dv, Float dh, v3<Float> const &position, v3<Float> const &view, v3<Float> const &up, v3<Float> const &right)
      const
  {
    auto const target = view + dh * right + dv * up;
    auto const dir = normalize(target);

    return {position, dir};
  }
};

} // namespace htracer::raytracing::lenses

#endif