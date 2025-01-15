#ifndef HTRACER_STAGING_SCENE_SCENE_HXX
#define HTRACER_STAGING_SCENE_SCENE_HXX


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
  emplace_object(Geometry<Float> &&geometry, material<Float> const &material);

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

} // namespace htracer::staging

#endif
