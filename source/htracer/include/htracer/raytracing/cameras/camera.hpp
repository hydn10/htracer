#ifndef HTRACER_RAYTRACING_CAMERA_HPP
#define HTRACER_RAYTRACING_CAMERA_HPP


#include <htracer/raytracing/image.hpp>
#include <htracer/raytracing/sampler.hpp>
#include <htracer/scene/scene_view.hpp>
#include <htracer/utils/randomness.hpp>
#include <htracer/vector.hpp>

#include <algorithm>
#include <cmath>
#include <execution>
#include <ranges>
#include <vector>


namespace htracer::raytracing
{

template<typename T, typename Float>
concept deterministic_lens = requires(T a) {
  {
    a.get_ray(
        Float{},
        Float{},
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>())
  } -> std::same_as<geometries::ray<Float>>;
};


template<typename T, typename Float>
concept deterministic_pixel_sampler = requires(T a) {
  { a.get_coords(uint32_t{}, uint32_t{}) } -> std::same_as<std::pair<Float, Float>>;
};


template<typename T, typename Float>
concept nondeterministic_lens = requires(T a, utils::randomness<> &r) {
  {
    a.get_ray(
        Float{},
        Float{},
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        r)
  } -> std::same_as<geometries::ray<Float>>;
};


template<typename T, typename Float>
concept nondeterministic_pixel_sampler = requires(T a, utils::randomness<> &r) {
  { a.get_coords(uint32_t{}, uint32_t{}, r) } -> std::same_as<std::pair<Float, Float>>;
};


template<typename T, typename Float>
concept lens = deterministic_lens<T, Float> || nondeterministic_lens<T, Float>;


template<typename T, typename Float>
concept pixel_sampler = deterministic_pixel_sampler<T, Float> || nondeterministic_pixel_sampler<T, Float>;


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
class sampling_camera
{
  template<utils::uniform_random_generator Generator>
  static auto
  get_coords(PixelSampler &pixel_sampler, uint32_t v_idx, uint32_t h_idx, utils::randomness<Generator> &rand);

  template<utils::uniform_random_generator Generator>
  static auto
  get_ray(
      Lens &lens,
      Float dh,
      Float dv,
      v3<Float> const &position,
      v3<Float> const &view,
      v3<Float> const &up,
      v3<Float> const &right,
      utils::randomness<Generator> &rand);

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
  sampling_camera(
      v3<Float> const &position,
      v3<Float> const &view,
      v3<Float> const &up,
      uint32_t horizontal_resolution,
      uint32_t vertical_resolution,
      Float fov,
      Lens &lens,
      PixelSampler &pixel_sampler);

  template<utils::uniform_random_generator Engine, template<typename> typename... Geometries>
  image<Float>
  render(scene::scene_view<scene::scene<Float, Geometries...>> scene, uint32_t samples, utils::randomness<Engine> &rand)
      const;

  template<typename ExecutionPolicy, utils::uniform_random_generator Engine, template<typename> typename... Geometries>
  image<Float>
  render(
      ExecutionPolicy &&policy,
      scene::scene_view<scene::scene<Float, Geometries...>> scene,
      uint32_t samples,
      utils::randomness<Engine> &rand) const;
};


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
template<utils::uniform_random_generator Generator>
auto
sampling_camera<Float, Lens, PixelSampler>::get_coords(
    PixelSampler &pixel_sampler, uint32_t v_idx, uint32_t h_idx, utils::randomness<Generator> &rand)
{
  if constexpr (deterministic_pixel_sampler<PixelSampler, Float>)
  {
    return pixel_sampler.get_coords(v_idx, h_idx);
  }
  else
  {
    return pixel_sampler.get_coords(v_idx, h_idx, rand);
  }
}


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
template<utils::uniform_random_generator Generator>
auto
sampling_camera<Float, Lens, PixelSampler>::get_ray(
    Lens &lens,
    Float dv,
    Float dh,
    v3<Float> const &position,
    v3<Float> const &view,
    v3<Float> const &up,
    v3<Float> const &right,
    utils::randomness<Generator> &rand)
{
  if constexpr (deterministic_lens<Lens, Float>)
  {
    return lens.get_ray(dv, dh, position, view, up, right);
  }
  else
  {
    return lens.get_ray(dv, dh, position, view, up, right, rand);
  }
}


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
sampling_camera<Float, Lens, PixelSampler>::sampling_camera(
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


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
template<utils::uniform_random_generator Engine, template<typename> typename... Geometries>
image<Float>
sampling_camera<Float, Lens, PixelSampler>::render(
    scene::scene_view<scene::scene<Float, Geometries...>> scene,
    uint32_t samples,
    utils::randomness<Engine> &rand) const
{
  return render(std::execution::unseq, scene, samples);
}


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
template<typename ExecutionPolicy, utils::uniform_random_generator Engine, template<typename> typename... Geometries>
image<Float>
sampling_camera<Float, Lens, PixelSampler>::render(
    ExecutionPolicy &&policy,
    scene::scene_view<scene::scene<Float, Geometries...>> scene,
    uint32_t samples,
    utils::randomness<Engine> &rand) const
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
        for (uint32_t j = 0; j < h_res_; ++j)
        {
          for (auto k = samples; k > 0; --k)
          {
            auto const [v_coord, h_coord] = get_coords(pixel_sampler_, i, j, rand);
            auto const dv = get_dv(v_coord);
            auto const dh = get_dh(h_coord);

            auto const ray = get_ray(lens_, dv, dh, position_, view_, up_, right_, rand);

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
