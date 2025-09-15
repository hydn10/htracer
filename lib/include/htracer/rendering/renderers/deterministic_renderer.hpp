#ifndef HTRACER_RENDERING_RENDERERS_DETERMINISTIC_RENDERER_HPP
#define HTRACER_RENDERING_RENDERERS_DETERMINISTIC_RENDERER_HPP


#include <htracer/colors/colors.hpp>
#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>
#include <htracer/rendering/image.hpp>
#include <htracer/rendering/samplers/deterministic_sampler.hpp>
#include <htracer/staging/scene.hpp>

#include <algorithm>
#include <vector>


namespace htracer::rendering::renderers
{

template<
    typename Float,
    typename Batcher,
    typename Scene,
    deterministic_sensor<Float> Sensor,
    deterministic_lens<Float> Lens>
class deterministic_renderer
{
  Batcher const &batcher_;
  Scene const &scene_;
  camera<Float> const &camera_;
  Sensor const &sensor_;
  Lens const &lens_;

public:
  deterministic_renderer(
      Batcher const &batcher,
      Scene const &scene,
      camera<Float> const &camera,
      Sensor const &sensor,
      Lens const &lens) noexcept;

  template<typename ExPolicy>
  [[nodiscard]]
  image<Float>
  render(ExPolicy &&) const;
};


template<
    typename Float,
    typename Batcher,
    typename Scene,
    deterministic_sensor<Float> Sensor,
    deterministic_lens<Float> Lens>
deterministic_renderer<Float, Batcher, Scene, Sensor, Lens>::deterministic_renderer(
    Batcher const &batcher,
    Scene const &scene,
    camera<Float> const &camera,
    Sensor const &sensor,
    Lens const &lens) noexcept
    : batcher_{batcher}
    , scene_{scene}
    , camera_{camera}
    , sensor_{sensor}
    , lens_{lens}
{
}


template<
    typename Float,
    typename Batcher,
    typename Scene,
    deterministic_sensor<Float> Sensor,
    deterministic_lens<Float> Lens>
template<typename ExPolicy>
image<Float>
deterministic_renderer<Float, Batcher, Scene, Sensor, Lens>::render(ExPolicy &&) const
{
  std::vector<colors::srgb_linear<Float>> pixels(camera_.v_res() * camera_.h_res());
  samplers::detail_::deterministic_sampler sampler;

  auto range = batcher_.make_range(camera_);
  auto accum = batcher_.make_accumulator(pixels, sampler, scene_, camera_, sensor_, lens_);

  constexpr auto std_policy = std::remove_cvref_t<ExPolicy>::get_std_policy();

  std::for_each(std_policy, std::begin(range), std::end(range), std::move(accum));

  return {camera_.h_res(), camera_.v_res(), std::move(pixels)};
}

} // namespace htracer::rendering::renderers

#endif
