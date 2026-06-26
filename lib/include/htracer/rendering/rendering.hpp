#ifndef HTRACER_RENDERING_RENDERING_HPP
#define HTRACER_RENDERING_RENDERING_HPP


#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>
#include <htracer/rendering/detail/component_ref.hpp>
#include <htracer/rendering/renderers/deterministic_renderer.hpp>
#include <htracer/rendering/renderers/randomized_renderer.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>

#include <utility>


namespace htracer::rendering
{

template<typename Float, typename Batcher, typename Sensor, typename Lens>
[[nodiscard]]
constexpr auto
make_renderer(camera<Float> camera_arg, Batcher &&batcher_arg, Sensor &&sensor_arg, Lens &&lens_arg)
    requires sensor<detail_::component_type<Sensor>, Float> && lens<detail_::component_type<Lens>, Float>
{
  using stored_batcher = detail_::stored_component<Batcher>;
  using stored_sensor = detail_::stored_component<Sensor>;
  using stored_lens = detail_::stored_component<Lens>;

  if constexpr (
      deterministic_sensor<detail_::component_type<stored_sensor>, Float>
      && deterministic_lens<detail_::component_type<stored_lens>, Float>)
  {
    return renderers::deterministic_renderer<Float, stored_batcher, stored_sensor, stored_lens>{
        std::move(camera_arg),
        std::forward<Batcher>(batcher_arg),
        std::forward<Sensor>(sensor_arg),
        std::forward<Lens>(lens_arg)};
  }
  else
  {
    return renderers::randomized_renderer<Float, stored_batcher, stored_sensor, stored_lens>{
        std::move(camera_arg),
        std::forward<Batcher>(batcher_arg),
        std::forward<Sensor>(sensor_arg),
        std::forward<Lens>(lens_arg)};
  }
}


template<typename ExPolicy, typename Scene, typename Float, typename Batcher, typename Sensor, typename Lens, typename... Args>
[[nodiscard]]
constexpr auto
render(
    ExPolicy &&policy,
    Scene const &scene,
    camera<Float> camera_arg,
    Batcher &&batcher_arg,
    Sensor &&sensor_arg,
    Lens &&lens_arg,
    Args &&...args)
    requires requires {
      make_renderer(
          std::declval<camera<Float>>(),
          std::declval<Batcher>(),
          std::declval<Sensor>(),
          std::declval<Lens>())
          .render(std::declval<ExPolicy>(), std::declval<Scene const &>(), std::declval<Args>()...);
    }
{
  auto renderer = make_renderer(
      std::move(camera_arg),
      std::forward<Batcher>(batcher_arg),
      std::forward<Sensor>(sensor_arg),
      std::forward<Lens>(lens_arg));

  return renderer.render(std::forward<ExPolicy>(policy), scene, std::forward<Args>(args)...);
}

} // namespace htracer::rendering

#endif
