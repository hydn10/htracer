#ifndef HTRACER_STAGING_SCENE_HPP
#define HTRACER_STAGING_SCENE_HPP


#include <htracer/geometries/plane.hpp>
#include <htracer/geometries/sphere.hpp>
#include <htracer/staging/scene_with.hpp>

#include <concepts>


namespace htracer::staging
{

template<std::floating_point Float>
class scene final : private scene_with<Float, geometries::plane, geometries::sphere>
{
  using scene_with<Float, geometries::plane, geometries::sphere>::emplace_object;

public:
  constexpr void
  emplace_sphere(geometries::sphere<Float> geometry, material<Float> material);

  constexpr void
  emplace_plane(geometries::plane<Float> geometry, material<Float> material);

  using scene_with<Float, geometries::plane, geometries::sphere>::add_light;
  using scene_with<Float, geometries::plane, geometries::sphere>::for_each_object;
  using scene_with<Float, geometries::plane, geometries::sphere>::lights;
};


template<std::floating_point Float>
constexpr void
scene<Float>::emplace_sphere(geometries::sphere<Float> sphere, material<Float> material)
{
  emplace_object(std::move(sphere), std::move(material));
}


template<std::floating_point Float>
constexpr void
scene<Float>::emplace_plane(geometries::plane<Float> plane, material<Float> material)
{
  emplace_object(std::move(plane), std::move(material));
}


} // namespace htracer::staging

#endif
