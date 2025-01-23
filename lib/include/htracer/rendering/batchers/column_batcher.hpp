#ifndef HTRACER_RENDERING_BATCHERS_COLUMN_BATCHER_HPP
#define HTRACER_RENDERING_BATCHERS_COLUMN_BATCHER_HPP


#include <htracer/colors/colors.hpp>
#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>
#include <htracer/staging/scene.hpp>

#include <concepts>
#include <ranges>
#include <vector>


namespace htracer::rendering::batchers
{

class column_batcher
{
public:
  template<std::floating_point Float, typename Sampler, typename Scene, sensor<Float> Sensor, lens<Float> Lens>
  auto
  make_accumulator(
      std::vector<colors::srgb_linear<Float>> &pixels,
      Sampler const &sampler,
      Scene const &scene,
      camera<Float> const &camera,
      Sensor const &sensor,
      Lens const &lens) const noexcept;

  template<std::floating_point Float>
  auto
  make_range(camera<Float> const &camera) const noexcept;
};


template<std::floating_point Float, typename Sampler, typename Scene, sensor<Float> Sensor, lens<Float> Lens>
auto
column_batcher::make_accumulator(
    std::vector<colors::srgb_linear<Float>> &pixels,
    Sampler const &sampler,
    Scene const &scene,
    camera<Float> const &camera,
    Sensor const &sensor,
    Lens const &lens) const noexcept
{
  return [&pixels, &sampler, &scene, &camera, &sensor, &lens](uint32_t h_idx)
  {
    auto const v_res = camera.v_res();
    auto const h_res = camera.h_res();

    for (auto const v_idx : std::views::iota(uint32_t{0}, v_res))
    {
      pixels[h_res * v_idx + h_idx] += sampler.render_pixel(v_idx, h_idx, scene, camera, sensor, lens);
    }
  };
}


template<std::floating_point Float>
auto
column_batcher::make_range(camera<Float> const &camera) const noexcept
{
  return std::views::iota(uint32_t{0}, camera.h_res());
}

} // namespace htracer::rendering::batchers

#endif
