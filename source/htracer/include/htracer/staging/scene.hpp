#ifndef HTRACER_STAGING_SCENE_HPP
#define HTRACER_STAGING_SCENE_HPP


#include <htracer/staging/light.hpp>
#include <htracer/staging/object.hpp>
#include <htracer/utils/heterogeneous_visitable.hpp>

#include <vector>


namespace htracer::staging
{

template<typename Float, template<typename> typename... Geometries>
class scene_view;


template<typename Float, template<typename> typename... Geometries>
class scene
{
  utils::heterogeneous_visitable<object<Float, Geometries>...> visitable_;
  std::vector<light<Float>> lights_;

public:
  // TODO: concept restrictions!
  template<template<typename> typename Geometry>
  constexpr void
  emplace_object(Geometry<Float> geometry, material<Float> material);

  constexpr void
  add_light(light<Float> const &light);

  template<typename F>
  void
  for_each_object(F &&f);
  template<typename F>
  void
  for_each_object(F &&f) const;
  constexpr auto const &
  lights() const;

  constexpr
  operator scene_view<Float, Geometries...>() const noexcept;
};


template<typename Float, template<typename> typename... Geometries>
template<template<typename> typename Geometry>
constexpr void
scene<Float, Geometries...>::emplace_object(Geometry<Float> geometry, material<Float> material)
{
  visitable_.template emplace<object<Float, Geometry>>(std::move(geometry), std::move(material));
}


template<typename Float, template<typename> typename... Geometries>
constexpr void
scene<Float, Geometries...>::add_light(light<Float> const &light)
{
  lights_.push_back(light);
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


template<typename Float, template<typename> typename... Geometries>
constexpr scene<Float, Geometries...>::operator scene_view<Float, Geometries...>() const noexcept
{
  return scene_view<Float, Geometries...>(*this);
}

} // namespace htracer::staging

#endif
