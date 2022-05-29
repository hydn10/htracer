#ifndef HTRACER_SCENE_OBJECT_BASE_HPP
#define HTRACER_SCENE_OBJECT_BASE_HPP


#include <htracer/geometries/geometry.hpp>
#include <htracer/scene/material.hpp>


namespace htracer::scene
{

template<typename Float>
class object_base
{
  scene::material<Float> material_;

public:
  object_base(material<Float> &&material);

  object_base(object_base<Float> const &) = default;
  object_base(object_base<Float> &&) = default;
  object_base<Float> &
  operator=(object_base<Float> const &) = default;
  object_base<Float> &
  operator=(object_base<Float> &&) = default;

  virtual ~object_base() = default;

  material<Float> const &
  get_material() const;

  virtual geometries::geometry<Float> &
  geometry() = 0;
  virtual geometries::geometry<Float> const &
  geometry() const = 0;
};


template<typename Float>
object_base<Float>::object_base(scene::material<Float> &&material)
    : material_{std::move(material)}
{
}


template<typename Float>
material<Float> const &
object_base<Float>::get_material() const
{
  return material_;
}

} // namespace htracer::scene

#endif
