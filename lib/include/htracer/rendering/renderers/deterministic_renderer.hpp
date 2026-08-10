#ifndef HTRACER_RENDERING_RENDERERS_DETERMINISTIC_RENDERER_HPP
#define HTRACER_RENDERING_RENDERERS_DETERMINISTIC_RENDERER_HPP


#include <htracer/colors/colors.hpp>
#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>
#include <htracer/rendering/detail/component_ref.hpp>
#include <htracer/rendering/image.hpp>
#include <htracer/rendering/policies.hpp>
#include <htracer/rendering/samplers/deterministic_sampler.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>

#include <algorithm>
#include <utility>
#include <vector>


#if defined(_MSC_VER)
#define HTRACER_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define HTRACER_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif


namespace htracer::rendering::renderers
{

template<typename Float, typename Batcher, typename Sensor, typename Lens>
requires deterministic_sensor<detail_::component_type<Sensor>, Float> &&
         deterministic_lens<detail_::component_type<Lens>, Float>
class deterministic_renderer
{
  camera<Float> camera_;
  HTRACER_NO_UNIQUE_ADDRESS Batcher batcher_;
  HTRACER_NO_UNIQUE_ADDRESS Sensor sensor_;
  HTRACER_NO_UNIQUE_ADDRESS Lens lens_;

public:
  constexpr deterministic_renderer(camera<Float> camera, Batcher batcher, Sensor sensor, Lens lens);

  template<rendering_policy ExPolicy, typename Scene>
  [[nodiscard]]
  image<Float>
  render(ExPolicy &&, Scene const &scene) const;

  template<rendering_policy ExPolicy, typename Scene>
  image<Float>
  render(ExPolicy &&, Scene const &, samples_per_pixel) const = delete;
};


template<typename Float, typename Batcher, typename Sensor, typename Lens>
requires deterministic_sensor<detail_::component_type<Sensor>, Float> &&
             deterministic_lens<detail_::component_type<Lens>, Float>
constexpr deterministic_renderer<Float, Batcher, Sensor, Lens>::deterministic_renderer(
    camera<Float> camera, Batcher batcher, Sensor sensor, Lens lens)
    : camera_{std::move(camera)}
    , batcher_{std::move(batcher)}
    , sensor_{std::move(sensor)}
    , lens_{std::move(lens)}
{
}


template<typename Float, typename Batcher, typename Sensor, typename Lens>
requires deterministic_sensor<detail_::component_type<Sensor>, Float> &&
         deterministic_lens<detail_::component_type<Lens>, Float>
template<rendering_policy ExPolicy, typename Scene>
image<Float>
deterministic_renderer<Float, Batcher, Sensor, Lens>::render(ExPolicy &&policy, Scene const &scene) const
{
  std::vector<colors::srgb_linear<Float>> pixels(camera_.v_res() * camera_.h_res());
  samplers::detail_::deterministic_sampler sampler;

  auto const &batcher = detail_::component_ref(batcher_);
  auto const &sensor = detail_::component_ref(sensor_);
  auto const &lens = detail_::component_ref(lens_);

  auto range = batcher.make_range(camera_);
  auto accum = batcher.make_accumulator(pixels, sampler, scene, camera_, sensor, lens);

  std::for_each(std::forward<ExPolicy>(policy), std::begin(range), std::end(range), std::move(accum));

  return {camera_.h_res(), camera_.v_res(), std::move(pixels)};
}

} // namespace htracer::rendering::renderers

#undef HTRACER_NO_UNIQUE_ADDRESS

#endif
