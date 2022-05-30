#ifndef HTRACER_SCENE_OBJECT_HPP
#define HTRACER_SCENE_OBJECT_HPP


#include <htracer/scene/material.hpp>
#include <htracer/scene/object_base.hpp>


namespace htracer::scene
{

template<typename Float, template<typename> typename Geometry>
class object final : public object_base<Float>
{
  Geometry<Float> geometry_;

public:
  object(Geometry<Float> const &geometry, material<Float> const &material);

  Geometry<Float> &
  get_geometry() override;
  Geometry<Float> const &
  get_geometry() const override;
};


template<typename Float, template<typename> typename Geometry>
object<Float, Geometry>::object(Geometry<Float> const &geometry, material<Float> const &material)
    : object_base<Float>(material)
    , geometry_{geometry}
{
}


template<typename Float, template<typename> typename Geometry>
Geometry<Float> &
object<Float, Geometry>::get_geometry()
{
  return geometry_;
}


template<typename Float, template<typename> typename Geometry>
Geometry<Float> const &
object<Float, Geometry>::get_geometry() const
{
  return geometry_;
}

} // namespace htracer::scene

#endif
