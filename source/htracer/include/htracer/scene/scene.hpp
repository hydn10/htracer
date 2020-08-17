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
  constexpr void
  add_object(object<Float, geometry::sphere>&& sphere);
  constexpr void
  add_object(object<Float, geometry::plane>&& plane);

  constexpr void
  add_light(light<Float>&& light);

  constexpr container const&
  objects() const;
  constexpr auto const&
  lights() const;
};


template<typename Float>
constexpr void
scene<Float>::add_object(object<Float, geometry::sphere>&& sphere)
{
  objects_.push(std::forward<object<Float, geometry::sphere>>(sphere));
}


template<typename Float>
constexpr void
scene<Float>::add_object(object<Float, geometry::plane>&& plane)
{
  objects_.push(std::forward<object<Float, geometry::plane>>(plane));
}


template<typename Float>
constexpr void
scene<Float>::add_light(light<Float>&& light)
{
  lights_.push_back(light);
}


template<typename Float>
constexpr auto
scene<Float>::objects() const -> container const&
{
  return objects_;
}


template<typename Float>
constexpr auto const&
scene<Float>::lights() const
{
  return lights_;
}

} // namespace htracer::scene

#endif // HTRACER_SCENE_SCENE_HPP
