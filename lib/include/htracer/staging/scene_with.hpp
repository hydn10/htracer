#ifndef HTRACER_STAGING_SCENE_WITH_HPP
#define HTRACER_STAGING_SCENE_WITH_HPP


#include <htracer/staging/light.hpp>
#include <htracer/staging/object.hpp>
#include <htracer/utils/heterogeneous_visitable.hpp>

#include <vector>


namespace htracer::staging
{

template<typename Float, template<typename> typename... Geometries>
class scene_with
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
  [[nodiscard]]
  constexpr auto const &
  lights() const noexcept;
};


template<typename Float, template<typename> typename... Geometries>
template<template<typename> typename Geometry>
constexpr void
scene_with<Float, Geometries...>::emplace_object(Geometry<Float> geometry, material<Float> material)
{
  visitable_.template emplace<object<Float, Geometry>>(std::move(geometry), std::move(material));
}


template<typename Float, template<typename> typename... Geometries>
constexpr void
scene_with<Float, Geometries...>::add_light(light<Float> const &light)
{
  lights_.push_back(light);
}


template<typename Float, template<typename> typename... Geometries>
template<typename F>
void
scene_with<Float, Geometries...>::for_each_object(F &&f)
{
  visitable_.visit(std::forward<F>(f));
}


template<typename Float, template<typename> typename... Geometries>
template<typename F>
void
scene_with<Float, Geometries...>::for_each_object(F &&f) const
{
  visitable_.visit(std::forward<F>(f));
}


template<typename Float, template<typename> typename... Geometries>
constexpr auto const &
scene_with<Float, Geometries...>::lights() const noexcept
{
  return lights_;
}

} // namespace htracer::staging

#endif