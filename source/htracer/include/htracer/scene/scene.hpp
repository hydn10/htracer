#ifndef HTRACER_SCENE_SCENE_HPP
#define HTRACER_SCENE_SCENE_HPP


#include <htracer/geometry/sphere.hpp>
#include <htracer/scene/light.hpp>
#include <htracer/scene/object.hpp>

#include <vector>


namespace htracer::scene
{
template<typename Float>
class scene
{
  std::vector<object<Float, geometry::sphere>> objects_;
  std::vector<light<Float>> lights_;

public:
  void
  add_object(object<Float, geometry::sphere>&& sphere)
  {
    objects_.push_back(sphere);
  }

  void
  add_light(light<Float>&& light)
  {
    lights_.push_back(light);
  }

  auto const&
  objects() const
  {
    return objects_;
  }

  auto const&
  lights() const
  {
    return lights_;
  }
};

} // namespace htracer::scene

#endif // HTRACER_SCENE_SCENE_HPP
