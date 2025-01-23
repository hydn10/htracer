#ifndef HTRACER_STAGING_OBJECT_BASE_HPP
#define HTRACER_STAGING_OBJECT_BASE_HPP


#include <htracer/geometries/geometry.hpp>
#include <htracer/staging/material.hpp>


namespace htracer::staging
{

template<typename Float>
class object_base
{
  material<Float> material_;

public:
  object_base(material<Float> material);
  virtual ~object_base() = default;

  object_base(object_base<Float> const &) = default;
  object_base(object_base<Float> &&) = default;
  object_base<Float> &
  operator=(object_base<Float> const &) = default;
  object_base<Float> &
  operator=(object_base<Float> &&) = default;

  material<Float> const &
  get_material() const;

  virtual geometries::geometry<Float> &
  get_geometry() = 0;
  virtual geometries::geometry<Float> const &
  get_geometry() const = 0;
};


template<typename Float>
object_base<Float>::object_base(material<Float> material)
    : material_{std::move(material)}
{
}


template<typename Float>
material<Float> const &
object_base<Float>::get_material() const
{
  return material_;
}

} // namespace htracer::staging

#endif
