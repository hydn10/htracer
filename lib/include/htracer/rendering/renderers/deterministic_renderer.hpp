#ifndef HTRACER_RENDERING_RENDERERS_DETERMINISTIC_RENDERER_HPP
#define HTRACER_RENDERING_RENDERERS_DETERMINISTIC_RENDERER_HPP


#include <htracer/colors/colors.hpp>
#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>
#include <htracer/rendering/detail/component_ref.hpp>
#include <htracer/rendering/image.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>
#include <htracer/rendering/samplers/deterministic_sampler.hpp>

#include <algorithm>
#include <type_traits>
#include <utility>
#include <vector>


namespace htracer::rendering::renderers
{

template<
    typename Float,
    typename Batcher,
    typename Sensor,
    typename Lens>
  requires deterministic_sensor<detail_::component_type<Sensor>, Float>
        && deterministic_lens<detail_::component_type<Lens>, Float>
class deterministic_renderer
{
  camera<Float> camera_;
  [[no_unique_address]] Batcher batcher_;
  [[no_unique_address]] Sensor sensor_;
  [[no_unique_address]] Lens lens_;

public:
  constexpr deterministic_renderer(camera<Float> camera, Batcher batcher, Sensor sensor, Lens lens);

  template<typename ExPolicy, typename Scene>
  [[nodiscard]]
  image<Float>
  render(ExPolicy &&, Scene const &scene) const;

  template<typename ExPolicy, typename Scene>
  image<Float>
  render(ExPolicy &&, Scene const &, samples_per_pixel) const = delete;
};


template<
    typename Float,
    typename Batcher,
    typename Sensor,
    typename Lens>
  requires deterministic_sensor<detail_::component_type<Sensor>, Float>
        && deterministic_lens<detail_::component_type<Lens>, Float>
constexpr deterministic_renderer<Float, Batcher, Sensor, Lens>::deterministic_renderer(
    camera<Float> camera, Batcher batcher, Sensor sensor, Lens lens)
    : camera_{std::move(camera)}
    , batcher_{std::move(batcher)}
    , sensor_{std::move(sensor)}
    , lens_{std::move(lens)}
{
}


template<
    typename Float,
    typename Batcher,
    typename Sensor,
    typename Lens>
  requires deterministic_sensor<detail_::component_type<Sensor>, Float>
        && deterministic_lens<detail_::component_type<Lens>, Float>
template<typename ExPolicy, typename Scene>
image<Float>
deterministic_renderer<Float, Batcher, Sensor, Lens>::render(ExPolicy &&, Scene const &scene) const
{
  std::vector<colors::srgb_linear<Float>> pixels(camera_.v_res() * camera_.h_res());
  samplers::detail_::deterministic_sampler sampler;

  auto const &batcher = detail_::component_ref(batcher_);
  auto const &sensor = detail_::component_ref(sensor_);
  auto const &lens = detail_::component_ref(lens_);

  auto range = batcher.make_range(camera_);
  auto accum = batcher.make_accumulator(pixels, sampler, scene, camera_, sensor, lens);

  constexpr auto std_policy = std::remove_cvref_t<ExPolicy>::get_std_policy();

  std::for_each(std_policy, std::begin(range), std::end(range), std::move(accum));

  return {camera_.h_res(), camera_.v_res(), std::move(pixels)};
}

} // namespace htracer::rendering::renderers

#endif
