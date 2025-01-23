#ifndef HTRACER_RENDERING_SAMPLERS_REPEAT_SAMPLER_HPP
#define HTRACER_RENDERING_SAMPLERS_REPEAT_SAMPLER_HPP


#include <htracer/colors/colors.hpp>
#include <htracer/rendering/adapters/randomized_adapter.hpp>
#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>
#include <htracer/staging/scene.hpp>


namespace htracer::rendering::samplers::detail_
{

template<typename GeneratorProvider>
class repeat_sampler
{
  uint32_t num_samples_;
  GeneratorProvider rep_;

public:
  repeat_sampler(uint32_t num_samples, GeneratorProvider rep) noexcept;

  template<typename Float, typename Scene, sensor<Float> Sensor, lens<Float> Lens>
  colors::srgb_linear<Float>
  render_pixel(
      uint32_t v_idx,
      uint32_t h_idx,
      Scene const &scene,
      camera<Float> const &camera,
      Sensor const &sensor,
      Lens const &lens) const;
};


template<typename GeneratorProvider>
repeat_sampler<GeneratorProvider>::repeat_sampler(uint32_t num_samples, GeneratorProvider rep) noexcept
    : num_samples_{num_samples}
    , rep_{std::move(rep)}
{
}

template<typename GeneratorProvider>
template<typename Float, typename Scene, sensor<Float> Sensor, lens<Float> Lens>
colors::srgb_linear<Float>
repeat_sampler<GeneratorProvider>::render_pixel(
    uint32_t v_idx,
    uint32_t h_idx,
    Scene const &scene,
    camera<Float> const &camera,
    Sensor const &sensor,
    Lens const &lens) const
{
  colors::srgb_linear<Float> accumulated{0, 0, 0};
  adapters::detail_::randomized_adapter<Float, Sensor, Lens, typename GeneratorProvider::generator_type> adapter{
      sensor, lens, rep_.get_generator()};

  for ([[maybe_unused]]
       auto const sample_idx : std::views::iota(uint32_t{0}, num_samples_))
  {
    auto const color = render_single_pixel(v_idx, h_idx, scene, camera, adapter);
    accumulated += color;
  }

  return accumulated * (Float{1} / num_samples_);
}


} // namespace htracer::rendering::samplers::detail_

#endif
