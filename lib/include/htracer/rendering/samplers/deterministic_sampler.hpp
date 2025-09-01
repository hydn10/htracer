#ifndef HTRACER_RENDERING_SAMPLERS_DETERMINISTIC_SAMPLER_HPP
#define HTRACER_RENDERING_SAMPLERS_DETERMINISTIC_SAMPLER_HPP


#include <htracer/colors/colors.hpp>
#include <htracer/rendering/adapters/deterministic_adapter.hpp>
#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>
#include <htracer/rendering/tracing.hpp>
#include <htracer/staging/scene.hpp>


namespace htracer::rendering::samplers::detail_
{

class deterministic_sampler
{
public:
  template<typename Float, typename Scene, deterministic_sensor<Float> Sensor, deterministic_lens<Float> Lens>
  [[nodiscard]]
  colors::srgb_linear<Float>
  render_pixel(
      uint32_t v_idx,
      uint32_t h_idx,
      Scene const &scene,
      camera<Float> const &camera,
      Sensor const &sensor,
      Lens const &lens) const;
};


template<typename Float, typename Scene, deterministic_sensor<Float> Sensor, deterministic_lens<Float> Lens>
colors::srgb_linear<Float>
deterministic_sampler::render_pixel(
    uint32_t v_idx,
    uint32_t h_idx,
    Scene const &scene,
    camera<Float> const &camera,
    Sensor const &sensor,
    Lens const &lens) const
{
  adapters::detail_::deterministic_adapter<Float, Sensor, Lens> adapter{sensor, lens};
  return render_single_pixel(v_idx, h_idx, scene, camera, adapter);
}

} // namespace htracer::rendering::samplers::detail_

#endif
