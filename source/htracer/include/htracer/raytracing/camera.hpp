#ifndef HTRACER_RAYTRACING_CAMERA_HPP
#define HTRACER_RAYTRACING_CAMERA_HPP


#include <htracer/raytracing/image.hpp>
#include <htracer/raytracing/sampler.hpp>
#include <htracer/scene/scene_view.hpp>
#include <htracer/vector.hpp>

#include <algorithm>
#include <cmath>
#include <ranges>
#include <execution>
#include <vector>


namespace htracer::raytracing
{

// clang-format off
template<typename T, typename Float>
concept camera_lens = requires(T a)
{
  {
    a.get_ray(
        Float{},
        Float{},
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>)
  }
  -> std::convertible_to<geometries::ray<Float>>;
};

template<typename T, typename Float>
concept camera_pixel_sampler = true;
// clang-format on


template<typename Float, camera_lens<Float> Lens, camera_pixel_sampler<Float> PixelSampler>
class camera
{
  v3<Float> position_;

  v3<Float> view_;
  v3<Float> up_;
  v3<Float> right_;

  uint32_t h_res_;
  uint32_t v_res_;
  Float fov_;

  Lens &lens_;
  PixelSampler &pixel_sampler_;

public:
  camera(
      v3<Float> const &position,
      v3<Float> const &view,
      v3<Float> const &up,
      uint32_t horizontal_resolution,
      uint32_t vertical_resolution,
      Float fov,
      Lens &lens,
      PixelSampler &pixel_sampler);

  template<template<typename> typename... Geometries>
  image<Float>
  render(scene::scene_view<scene::scene<Float, Geometries...>> scene, uint32_t samples) const;

  template<typename ExecutionPolicy, template<typename> typename... Geometries>
  image<Float>
  render(ExecutionPolicy &&policy, scene::scene_view<scene::scene<Float, Geometries...>> scene, uint32_t samples) const;
};


template<typename Float, camera_lens<Float> Lens, camera_pixel_sampler<Float> PixelSampler>
camera<Float, Lens, PixelSampler>::camera(
    v3<Float> const &position,
    v3<Float> const &view,
    v3<Float> const &up,
    uint32_t horizontal_resolution,
    uint32_t vertical_resolution,
    Float fov,
    Lens &lens,
    PixelSampler &pixel_sampler)
    : position_{position}
    , h_res_{horizontal_resolution}
    , v_res_{vertical_resolution}
    , fov_{fov}
    , lens_{lens}
    , pixel_sampler_{pixel_sampler}
{
  view_ = normalize(view);
  right_ = normalize(cross(view, up));
  up_ = cross(right_, view_);
}


template<typename Float, camera_lens<Float> Lens, camera_pixel_sampler<Float> PixelSampler>
template<template<typename> typename... Geometries>
image<Float>
camera<Float, Lens, PixelSampler>::render(
    scene::scene_view<scene::scene<Float, Geometries...>> scene, uint32_t samples) const
{
  return render(std::execution::unseq, scene, samples);
}


template<typename Float, camera_lens<Float> Lens, camera_pixel_sampler<Float> PixelSampler>
template<typename ExecutionPolicy, template<typename> typename... Geometries>
image<Float>
camera<Float, Lens, PixelSampler>::render(
    ExecutionPolicy &&policy, scene::scene_view<scene::scene<Float, Geometries...>> scene, uint32_t samples) const
{
  auto const v_tan = std::tan(fov_);
  auto const h_tan = v_tan * h_res_ / v_res_;

  std::vector<colors::srgb_linear<Float>> pixels(v_res_ * h_res_);

  std::vector<uint32_t> v_indices(v_res_);
  std::iota(std::begin(v_indices), std::end(v_indices), 0);

  auto get_dv = [v_tan, v_res = v_res_](Float idx)
  {
    return v_tan * (1 - ((2 * idx) / (v_res - 1)));
  };

  auto get_dh = [h_tan, h_res = h_res_](Float idx)
  {
    return h_tan * (((2 * idx) / (h_res - 1)) - 1);
  };

  std::for_each(
      policy,
      std::begin(v_indices),
      std::end(v_indices),
      [&](auto const i)
      {
        for (auto j = 0u; j < h_res_; ++j)
        {
          for (auto k = samples; k > 0; --k)
          {
            auto const [v_coord, h_coord] = pixel_sampler_.get_coords(i, j);
            auto const dv = get_dv(v_coord);
            auto const dh = get_dh(h_coord);

            auto const ray = lens_.get_ray(dv, dh, position_, view_, up_, right_);

            pixels[h_res_ * i + j] += sample(ray, scene);
          }
        }
      });

  for (auto &pixel : pixels)
  {
    auto const scale = Float{1} / samples;
    pixel *= scale;
  }

  return {h_res_, v_res_, std::move(pixels)};
}

} // namespace htracer::raytracing

#endif