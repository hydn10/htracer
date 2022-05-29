#ifndef HTRACER_SCENE_SCENE_HPP
#define HTRACER_SCENE_SCENE_HPP


#include <htracer/geometries/plane.hpp>
#include <htracer/geometries/sphere.hpp>
#include <htracer/scene/light.hpp>
#include <htracer/scene/object.hpp>
#include <htracer/utils/heterogeneous_visitable.hpp>

#include <vector>


namespace htracer::scene
{
template<typename Float, template<typename> typename... Geometries>
class scene
{
  utils::heterogeneous_visitable<object<Float, Geometries>...> visitable_;
  std::vector<light<Float>> lights_;

public:
  template<template<typename> typename Geometry>
  // TODO: concept restrictions!
  constexpr void
  emplace_object(Geometry<Float> &&geometry, material<Float> &&material);
  template<template<typename> typename Geometry>
  // TODO: concept restrictions!
  constexpr void
  emplace_object(Geometry<Float> &&geometry, material<Float> const &material);

  constexpr void
  add_light(light<Float> &&light);

  template<typename F>
  void
  for_each_object(F &&f);
  template<typename F>
  void
  for_each_object(F &&f) const;
  constexpr auto const &
  lights() const;
};


template<typename Float, template<typename> typename... Geometries>
template<template<typename> typename Geometry>
constexpr void
scene<Float, Geometries...>::emplace_object(Geometry<Float> &&geometry, material<Float> &&material)
{
  visitable_.emplace<object<Float, Geometry>>(std::forward<Geometry<Float>>(geometry), std::move(material));
}


template<typename Float, template<typename> typename... Geometries>
template<template<typename> typename Geometry>
constexpr void
scene<Float, Geometries...>::emplace_object(Geometry<Float> &&geometry, material<Float> const &material)
{
  visitable_.emplace<object<Float, Geometry>>(std::forward<Geometry<Float>>(geometry), material);
}


template<typename Float, template<typename> typename... Geometries>
constexpr void
scene<Float, Geometries...>::add_light(light<Float> &&light)
{
  lights_.push_back(std::move(light));
}


template<typename Float, template<typename> typename... Geometries>
template<typename F>
void
scene<Float, Geometries...>::for_each_object(F &&f)
{
  visitable_.visit(std::forward<F>(f));
}

template<typename Float, template<typename> typename... Geometries>
template<typename F>
void
scene<Float, Geometries...>::for_each_object(F &&f) const
{
  visitable_.visit(std::forward<F>(f));
}


template<typename Float, template<typename> typename... Geometries>
constexpr auto const &
scene<Float, Geometries...>::lights() const
{
  return lights_;
}

} // namespace htracer::scene

#endif
