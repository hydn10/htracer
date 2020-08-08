#ifndef HTRACER_SCENE_SCENE_HPP
#define HTRACER_SCENE_SCENE_HPP


#include <htracer/geometry/plane.hpp>
#include <htracer/geometry/sphere.hpp>
#include <htracer/scene/light.hpp>
#include <htracer/scene/object.hpp>
#include <htracer/utils/container.hpp>

#include <vector>


namespace htracer::scene
{
template<typename Float>
class scene
{
public:
  using container = utils::container<
      object<Float, geometry::sphere>,
      object<Float, geometry::plane>>;

private:
  container objects_;
  std::vector<light<Float>> lights_;

public:
  void
  add_object(object<Float, geometry::sphere>&& sphere);
  void
  add_object(object<Float, geometry::plane>&& plane);

  void
  add_light(light<Float>&& light);

  container const&
  objects() const;
  auto const&
  lights() const;
};


template<typename Float>
void
scene<Float>::add_object(object<Float, geometry::sphere>&& sphere)
{
  objects_.push(std::forward<object<Float, geometry::sphere>>(sphere));
}


template<typename Float>
void
scene<Float>::add_object(object<Float, geometry::plane>&& plane)
{
  objects_.push(std::forward<object<Float, geometry::plane>>(plane));
}


template<typename Float>
void
scene<Float>::add_light(light<Float>&& light)
{
  lights_.push_back(light);
}


template<typename Float>
auto
scene<Float>::objects() const -> container const&
{
  return objects_;
}


template<typename Float>
auto const&
scene<Float>::lights() const
{
  return lights_;
}


} // namespace htracer::scene

#endif // HTRACER_SCENE_SCENE_HPP
