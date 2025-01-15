#ifndef HTRACER_STAGING_SCENE_SCENE_IMPL_HXX
#define HTRACER_STAGING_SCENE_SCENE_IMPL_HXX


#include <htracer/staging/scene/scene.hxx>
#include <htracer/staging/scene/scene_view.hxx>


namespace htracer::staging
{

template<typename Float, template<typename> typename... Geometries>
template<template<typename> typename Geometry>
constexpr void
scene<Float, Geometries...>::emplace_object(Geometry<Float> &&geometry, material<Float> const &material)
{
  visitable_.template emplace<object<Float, Geometry>>(std::forward<Geometry<Float>>(geometry), material);
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
