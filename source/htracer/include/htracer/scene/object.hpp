#ifndef HTRACER_SCENE_OBJECT_HPP
#define HTRACER_SCENE_OBJECT_HPP


#include <htracer/scene/material.hpp>


namespace htracer::scene
{
template<typename Float, template<typename> typename Geometry>
class object
{
public:
  Geometry<Float> geometry;
  material<Float> mat;
};

} // namespace htracer::scene

#endif // HTRACER_SCENE_OBJECT_HPP
