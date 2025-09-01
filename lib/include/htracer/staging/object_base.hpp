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
  constexpr object_base(material<Float> material) noexcept;
  virtual ~object_base() = default;

  object_base(object_base<Float> const &) noexcept = default;
  object_base(object_base<Float> &&) noexcept = default;
  object_base<Float> &
  operator=(object_base<Float> const &) noexcept = default;
  object_base<Float> &
  operator=(object_base<Float> &&) noexcept = default;

  constexpr material<Float> const &
  get_material() const noexcept;

  virtual constexpr geometries::geometry<Float> &
  get_geometry() noexcept = 0;
  [[nodiscard]]
  virtual constexpr geometries::geometry<Float> const &
  get_geometry() const noexcept = 0;
};


template<typename Float>
constexpr object_base<Float>::object_base(material<Float> material) noexcept
    : material_{std::move(material)}
{
}


template<typename Float>
constexpr material<Float> const &
object_base<Float>::get_material() const noexcept
{
  return material_;
}

} // namespace htracer::staging

#endif