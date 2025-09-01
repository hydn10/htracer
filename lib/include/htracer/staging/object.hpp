#ifndef HTRACER_STAGING_OBJECT_HPP
#define HTRACER_STAGING_OBJECT_HPP


#include <htracer/staging/material.hpp>
#include <htracer/staging/object_base.hpp>


namespace htracer::staging
{

template<typename Float, template<typename> typename Geometry>
class object final : public object_base<Float>
{
  Geometry<Float> geometry_;

public:
  constexpr object(Geometry<Float> geometry, material<Float> material) noexcept;

    constexpr Geometry<Float> &
  get_geometry() noexcept override;
  constexpr Geometry<Float> const &
  get_geometry() const noexcept override;
};


template<typename Float, template<typename> typename Geometry>
constexpr object<Float, Geometry>::object(Geometry<Float> geometry, material<Float> material) noexcept
    : object_base<Float>(std::move(material))
    , geometry_{std::move(geometry)}
{
}


template<typename Float, template<typename> typename Geometry>
constexpr Geometry<Float> &
object<Float, Geometry>::get_geometry() noexcept
{
  return geometry_;
}


template<typename Float, template<typename> typename Geometry>
constexpr Geometry<Float> const &
object<Float, Geometry>::get_geometry() const noexcept
{
  return geometry_;
}

} // namespace htracer::staging

#endif