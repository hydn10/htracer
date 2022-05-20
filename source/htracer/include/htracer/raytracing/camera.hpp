#ifndef HTRACER_RAYTRACING_CAMERA_HPP
#define HTRACER_RAYTRACING_CAMERA_HPP


#include <htracer/raytracing/image.hpp>
#include <htracer/raytracing/sampler.hpp>
#include <htracer/scene/scene_view.hpp>
#include <htracer/utils/random.hpp>
#include <htracer/vector.hpp>

#include <algorithm>
#include <cmath>
#include <execution>
#include <numbers>
#include <ranges>
#include <vector>


namespace htracer::raytracing
{

template<typename Float>
class camera
{
  v3<Float> position_;

  v3<Float> view_;
  v3<Float> up_;
  v3<Float> right_;

  uint32_t h_res_;
  uint32_t v_res_;
  Float fov_;
  Float aperture_radius_;
  Float focal_distance_;

public:
  camera(
      v3<Float> const &position,
      v3<Float> const &view,
      v3<Float> const &up,
      uint32_t horizontal_resolution,
      uint32_t vertical_resolution,
      Float fov,
      Float aperture_radius,
      Float focal_distance);

  template<template<typename> typename... Geometries>
  image<Float>
  render(scene::scene_view<scene::scene<Float, Geometries...>> scene, uint32_t samples) const;

  template<typename ExecutionPolicy, template<typename> typename... Geometries>
  image<Float>
  render(ExecutionPolicy &&policy, scene::scene_view<scene::scene<Float, Geometries...>> scene, uint32_t samples) const;
};


template<typename Float>
camera<Float>::camera(
    v3<Float> const &position,
    v3<Float> const &view,
    v3<Float> const &up,
    uint32_t horizontal_resolution,
    uint32_t vertical_resolution,
    Float fov,
    Float aperture_radius,
    Float focal_distance)
    : position_{position}
    , h_res_{horizontal_resolution}
    , v_res_{vertical_resolution}
    , fov_{fov}
    , aperture_radius_{aperture_radius}
    , focal_distance_{focal_distance}
{
  view_ = normalize(view);
  right_ = normalize(cross(view, up));
  up_ = cross(right_, view_);
}


template<typename Float>
template<template<typename> typename... Geometries>
image<Float>
camera<Float>::render(scene::scene_view<scene::scene<Float, Geometries...>> scene, uint32_t samples) const
{
  return render(std::execution::unseq, scene, samples);
}


template<typename Float>
template<typename ExecutionPolicy, template<typename> typename... Geometries>
image<Float>
camera<Float>::render(
    ExecutionPolicy &&policy, scene::scene_view<scene::scene<Float, Geometries...>> scene, uint32_t samples) const
{
  const auto h_tan = std::tan(fov_);
  const auto v_tan = h_tan * v_res_ / h_res_;

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

  utils::random<Float> rand_offset(-.5, .5);
  utils::random<Float> rand_unit(0, 1);

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
            auto const dv = get_dv(i + rand_offset());
            auto const dh = get_dh(j + rand_offset());

            constexpr Float PI = std::numbers::pi_v<Float>;

            // TODO: Optimize if radius = 0.
            //       Maybe pass a templated object Lens that returns an origin given a position?
            //       Delve into different lens types (fisheye?) so I know what I do have to abstract.

            auto const angle = 2 * PI * rand_unit();
            auto const radius = aperture_radius_ * std::sqrt(rand_unit());

            auto const deviation = radius * (std::cos(angle) * right_ + std::sin(angle) * up_);

            auto const origin = position_ + deviation;

            auto const dir = normalize((focal_distance_ * (view_ + dh * right_ + dv * up_) - deviation));

            pixels[h_res_ * i + j] += sample({origin, dir}, scene);
          }
        }
      });

  for (auto &pixel : pixels)
  {
    const auto scale = Float{1} / samples;
    pixel *= scale;
  }

  return {h_res_, v_res_, std::move(pixels)};
}

} // namespace htracer::raytracing

#endif