#ifndef HTRACER_RAYTRACING_CAMERA_HPP
#define HTRACER_RAYTRACING_CAMERA_HPP


#include <htracer/raytracing/image.hpp>
#include <htracer/raytracing/sampler.hpp>
#include <htracer/scene/scene_view.hpp>
#include <htracer/vector.hpp>

#include <cmath>
#include <ostream>
#include <vector>


namespace htracer::raytracing
{

template<typename Float>
class camera
{
  v3<Float> origin_;

  v3<Float> view_;
  v3<Float> up_;
  v3<Float> right_;

  uint32_t h_res_;
  uint32_t v_res_;
  Float fov_;

public:
  camera(
      v3<Float> origin,
      const v3<Float> &view,
      const v3<Float> &up,
      uint32_t horizontal_resolution,
      uint32_t vertical_resolution,
      Float fov);

  template<template<typename> typename... Geometries>
  image<Float>
  render(scene::scene_view<scene::scene<Float, Geometries...>> scene) const;
};


template<typename Float>
camera<Float>::camera(
    v3<Float> origin,
    const v3<Float> &view,
    const v3<Float> &up,
    uint32_t horizontal_resolution,
    uint32_t vertical_resolution,
    Float fov)
    : origin_{origin}
    , h_res_{horizontal_resolution}
    , v_res_{vertical_resolution}
    , fov_{fov}
{
  view_ = view.normalized();
  right_ = cross(view, up).normalized();
  up_ = cross(right_, view_);
}


template<typename Float>
template<template<typename> typename... Geometries>
image<Float>
camera<Float>::render(scene::scene_view<scene::scene<Float, Geometries...>> scene) const
{
  const auto h_tan = std::tan(fov_);
  const auto v_tan = h_tan * v_res_ / h_res_;

  std::vector<colors::srgb_linear<Float>> pixels;
  pixels.reserve(v_res_ * h_res_);

  for (auto i = 0u; i < v_res_; ++i)
  {
    auto dv = v_tan * (1 - ((Float{2} * i) / (v_res_ - 1)));

    for (auto j = 0u; j < h_res_; ++j)
    {
      auto dh = h_tan * (((Float{2} * j) / (h_res_ - 1)) - 1);

      auto dir = view_ + dv * up_ + dh * right_;
      dir = dir.normalized();

      // TODO: emplace_back() ?
      pixels.push_back(sample<Float>({origin_, dir}, scene));
    }
  }

  return {h_res_, v_res_, std::move(pixels)};
}

} // namespace htracer::raytracing

#endif