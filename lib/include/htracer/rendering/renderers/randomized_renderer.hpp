#ifndef HTRACER_RENDERING_RENDERERS_RANDOMIZED_RENDERER_HPP
#define HTRACER_RENDERING_RENDERERS_RANDOMIZED_RENDERER_HPP


#include <htracer/colors/colors.hpp>
#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>
#include <htracer/rendering/detail/component_ref.hpp>
#include <htracer/rendering/image.hpp>
#include <htracer/rendering/policies.hpp>
#include <htracer/rendering/random_engine.hpp>
#include <htracer/rendering/random_seed.hpp>
#include <htracer/rendering/samplers/repeat_sampler.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>
#include <htracer/utils/generator_providers/seeded_provider.hpp>
#include <htracer/utils/generator_providers/thread_local_provider.hpp>
#include <htracer/utils/generator_providers/wrapping_provider.hpp>

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <random>
#include <type_traits>
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
requires sensor<detail_::component_type<Sensor>, Float> && lens<detail_::component_type<Lens>, Float>
class randomized_renderer
{
  camera<Float> camera_;
  HTRACER_NO_UNIQUE_ADDRESS Batcher batcher_;
  HTRACER_NO_UNIQUE_ADDRESS Sensor sensor_;
  HTRACER_NO_UNIQUE_ADDRESS Lens lens_;

  template<rendering_policy ExPolicy, typename Scene, typename GeneratorProvider>
  image<Float>
  render(ExPolicy &&, Scene const &scene, samples_per_pixel samples, GeneratorProvider &&rep) const;

public:
  constexpr randomized_renderer(camera<Float> camera, Batcher batcher, Sensor sensor, Lens lens);

  template<rendering_policy ExPolicy, typename Scene>
  [[nodiscard]]
  image<Float>
  render(ExPolicy &&policy, Scene const &scene, samples_per_pixel samples) const;

  template<rendering_policy ExPolicy, typename Scene>
  [[nodiscard]]
  image<Float>
  render(ExPolicy &&policy, Scene const &scene, samples_per_pixel samples, random_seed seed) const;

  template<rendering_policy ExPolicy, typename Scene>
  image<Float>
  render(ExPolicy &&, Scene const &) const = delete;
};


template<typename Float, typename Batcher, typename Sensor, typename Lens>
requires sensor<detail_::component_type<Sensor>, Float> && lens<detail_::component_type<Lens>, Float>
constexpr randomized_renderer<Float, Batcher, Sensor, Lens>::randomized_renderer(
    camera<Float> camera, Batcher batcher, Sensor sensor, Lens lens)
    : camera_{std::move(camera)}
    , batcher_{std::move(batcher)}
    , sensor_{std::move(sensor)}
    , lens_{std::move(lens)}
{
}


template<typename Float, typename Batcher, typename Sensor, typename Lens>
requires sensor<detail_::component_type<Sensor>, Float> && lens<detail_::component_type<Lens>, Float>
template<rendering_policy ExPolicy, typename Scene, typename GeneratorProvider>
image<Float>
randomized_renderer<Float, Batcher, Sensor, Lens>::render(
    ExPolicy &&policy, Scene const &scene, samples_per_pixel samples, GeneratorProvider &&rep) const
{
  std::vector<colors::srgb_linear<Float>> pixels(camera_.v_res() * camera_.h_res());
  samplers::detail_::repeat_sampler sampler(samples.value(), std::forward<GeneratorProvider>(rep));

  auto const &batcher = detail_::component_ref(batcher_);
  auto const &sensor = detail_::component_ref(sensor_);
  auto const &lens = detail_::component_ref(lens_);

  auto range = batcher.make_range(camera_);
  auto accum = batcher.make_accumulator(pixels, sampler, scene, camera_, sensor, lens);

  std::for_each(std::forward<ExPolicy>(policy), std::begin(range), std::end(range), std::move(accum));

  return {camera_.h_res(), camera_.v_res(), std::move(pixels)};
}


template<typename Float, typename Batcher, typename Sensor, typename Lens>
requires sensor<detail_::component_type<Sensor>, Float> && lens<detail_::component_type<Lens>, Float>
template<rendering_policy ExPolicy, typename Scene>
image<Float>
randomized_renderer<Float, Batcher, Sensor, Lens>::render(
    ExPolicy &&policy, Scene const &scene, samples_per_pixel samples) const
{
  // Use thread-safe engine only if needed.
  if constexpr (std::same_as<std::remove_cvref_t<ExPolicy>, parallel_policy>)
  {
    utils::generator_providers::detail_::thread_local_provider<random_engine> random_engine_provider;
    return render(std::forward<ExPolicy>(policy), scene, samples, std::move(random_engine_provider));
  }
  else
  {
    random_engine gen(std::random_device{}());
    utils::generator_providers::detail_::wrapping_provider<random_engine> random_engine_provider(gen);

    return render(std::forward<ExPolicy>(policy), scene, samples, std::move(random_engine_provider));
  }
}


template<typename Float, typename Batcher, typename Sensor, typename Lens>
requires sensor<detail_::component_type<Sensor>, Float> && lens<detail_::component_type<Lens>, Float>
template<rendering_policy ExPolicy, typename Scene>
image<Float>
randomized_renderer<Float, Batcher, Sensor, Lens>::render(
    ExPolicy &&policy, Scene const &scene, samples_per_pixel samples, random_seed seed) const
{
  utils::generator_providers::detail_::seeded_provider<random_engine> random_engine_provider(seed);
  return render(std::forward<ExPolicy>(policy), scene, samples, std::move(random_engine_provider));
}

} // namespace htracer::rendering::renderers

#undef HTRACER_NO_UNIQUE_ADDRESS

#endif
