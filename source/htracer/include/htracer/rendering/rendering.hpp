#ifndef HTRACER_RENDERING_RENDERING_HPP
#define HTRACER_RENDERING_RENDERING_HPP


#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>
#include <htracer/rendering/renderers/deterministic_renderer.hpp>
#include <htracer/rendering/renderers/randomized_renderer.hpp>
#include <htracer/staging/scene.hpp>


namespace htracer::rendering
{

template<
    typename Float,
    typename Batcher,
    sensor<Float> Sensor,
    lens<Float> Lens,
    template<typename>
    typename... Geometries>
auto
make_renderer(
    Batcher const &batcher,
    staging::scene<Float, Geometries...> const &scene,
    camera<Float> const &camera,
    Sensor const &sensor,
    Lens const &lens) noexcept;


template<
    typename Float,
    typename Batcher,
    sensor<Float> Sensor,
    lens<Float> Lens,
    template<typename>
    typename... Geometries>
auto
make_renderer(
    Batcher const &batcher,
    staging::scene<Float, Geometries...> const &scene,
    camera<Float> const &camera,
    Sensor const &sensor,
    Lens const &lens) noexcept
{
  if constexpr (deterministic_sensor<Sensor, Float> && deterministic_lens<Lens, Float>)
  {
    return renderers::deterministic_renderer<Float, Batcher, Sensor, Lens, Geometries...>(
        batcher, scene, camera, sensor, lens);
  }
  else
  {
    return renderers::randomized_renderer<Float, Batcher, Sensor, Lens, Geometries...>(
        batcher, scene, camera, sensor, lens);
  }
}

} // namespace htracer::rendering

#endif
