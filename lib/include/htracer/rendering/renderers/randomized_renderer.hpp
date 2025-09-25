#ifndef HTRACER_RENDERING_RENDERERS_RANDOMIZED_RENDERER_HPP
#define HTRACER_RENDERING_RENDERERS_RANDOMIZED_RENDERER_HPP


#include <htracer/colors/colors.hpp>
#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>
#include <htracer/rendering/image.hpp>
#include <htracer/rendering/samplers/repeat_sampler.hpp>
#include <htracer/staging/scene.hpp>
#include <htracer/utils/generator_providers/thread_local_provider.hpp>
#include <htracer/utils/generator_providers/wrapping_provider.hpp>

#include <algorithm>
#include <type_traits>
#include <vector>


namespace htracer::rendering::renderers
{

template<typename Float, typename Batcher, typename Scene, sensor<Float> Sensor, lens<Float> Lens>
class randomized_renderer
{
  Batcher const &batcher_;
  Scene const &scene_;
  camera<Float> const &camera_;
  Sensor const &sensor_;
  Lens const &lens_;

  template<typename ExPolicy, typename GeneratorProvider>
  image<Float>
  render(ExPolicy &&, uint32_t num_samples, GeneratorProvider &&rep) const;

public:
  randomized_renderer(
      Batcher const &batcher,
      Scene const &scene,
      camera<Float> const &camera,
      Sensor const &sensor,
      Lens const &lens) noexcept;

  template<typename ExPolicy>
  [[nodiscard]]
  image<Float>
  render(ExPolicy &&policy, uint32_t num_samples) const;
};


template<typename Float, typename Batcher, typename Scene, sensor<Float> Sensor, lens<Float> Lens>
randomized_renderer<Float, Batcher, Scene, Sensor, Lens>::randomized_renderer(
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


template<typename Float, typename Batcher, typename Scene, sensor<Float> Sensor, lens<Float> Lens>
template<typename ExPolicy, typename GeneratorProvider>
image<Float>
randomized_renderer<Float, Batcher, Scene, Sensor, Lens>::render(
    ExPolicy &&, uint32_t num_samples, GeneratorProvider &&rep) const
{
  std::vector<colors::srgb_linear<Float>> pixels(camera_.v_res() * camera_.h_res());
  samplers::detail_::repeat_sampler sampler(num_samples, std::forward<GeneratorProvider>(rep));

  auto range = batcher_.make_range(camera_);
  auto accum = batcher_.make_accumulator(pixels, sampler, scene_, camera_, sensor_, lens_);

  constexpr auto std_policy = std::remove_cvref_t<ExPolicy>::get_std_policy();

  std::for_each(std_policy, std::begin(range), std::end(range), std::move(accum));

  return {camera_.h_res(), camera_.v_res(), std::move(pixels)};
}


template<typename Float, typename Batcher, typename Scene, sensor<Float> Sensor, lens<Float> Lens>
template<typename ExPolicy>
image<Float>
randomized_renderer<Float, Batcher, Scene, Sensor, Lens>::render(ExPolicy &&policy, uint32_t num_samples) const
{
  // Use thread-safe engine only if needed.
  if constexpr (std::remove_cvref_t<ExPolicy>::is_parallel)
  {
    utils::generator_providers::detail_::thread_local_provider random_engine_provider;
    return render(std::forward<ExPolicy>(policy), num_samples, std::move(random_engine_provider));
  }
  else
  {
    std::default_random_engine gen;
    utils::generator_providers::detail_::wrapping_provider random_engine_provider(gen);

    return render(std::forward<ExPolicy>(policy), num_samples, std::move(random_engine_provider));
  }
}

} // namespace htracer::rendering::renderers

#endif
