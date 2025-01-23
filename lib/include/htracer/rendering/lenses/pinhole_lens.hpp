#ifndef HTRACER_RENDERING_LENSES_PINHOLE_LENS_HPP
#define HTRACER_RENDERING_LENSES_PINHOLE_LENS_HPP


#include <htracer/geometries/ray.hpp>
#include <htracer/vector.hpp>


namespace htracer::rendering::lenses
{

template<typename Float>
class pinhole_lens
{
public:
  geometries::ray<Float>
  get_ray(
      Float dv, Float dh, v3<Float> const &position, v3<Float> const &view, v3<Float> const &up, v3<Float> const &right)
      const;
};


template<typename Float>
geometries::ray<Float>
pinhole_lens<Float>::get_ray(
    Float dv, Float dh, v3<Float> const &position, v3<Float> const &view, v3<Float> const &up, v3<Float> const &right)
    const
{
  auto const target = view + dh * right + dv * up;
  auto const dir = normalize(target);

  return {position, dir};
}

} // namespace htracer::rendering::lenses

#endif
