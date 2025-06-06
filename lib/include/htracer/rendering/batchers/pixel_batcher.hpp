#ifndef HTRACER_RENDERING_BATCHERS_PIXEL_BATCHER_HPP
#define HTRACER_RENDERING_BATCHERS_PIXEL_BATCHER_HPP


#include <htracer/colors/colors.hpp>
#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>
#include <htracer/staging/scene.hpp>

#include <concepts>
#include <ranges>
#include <vector>


namespace htracer::rendering::batchers
{

class pixel_batcher
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
  constexpr auto
  make_range(camera<Float> const &camera) const noexcept;
};


template<std::floating_point Float, typename Sampler, typename Scene, sensor<Float> Sensor, lens<Float> Lens>
auto
pixel_batcher::make_accumulator(
    std::vector<colors::srgb_linear<Float>> &pixels,
    Sampler const &sampler,
    Scene const &scene,
    camera<Float> const &camera,
    Sensor const &sensor,
    Lens const &lens) const noexcept
{
  return [&pixels, &sampler, &scene, &camera, &sensor, &lens](auto index_pair)
  {
    auto [v_idx, h_idx] = index_pair;
    auto const h_res = camera.h_res();

    pixels[h_res * v_idx + h_idx] += sampler.render_pixel(v_idx, h_idx, scene, camera, sensor, lens);
  };
}


template<std::floating_point Float>
constexpr auto
pixel_batcher::make_range(camera<Float> const &camera) const noexcept
{
  constexpr uint32_t zero{0};

  return std::views::cartesian_product(std::views::iota(zero, camera.v_res()), std::views::iota(zero, camera.h_res()));
}

} // namespace htracer::rendering::batchers

#endif
