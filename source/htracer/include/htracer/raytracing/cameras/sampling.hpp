#ifndef HTRACER_RAYTRACING_CAMERAS_SAMPLING_HPP
#define HTRACER_RAYTRACING_CAMERAS_SAMPLING_HPP


#include <htracer/raytracing/concepts.hpp>
#include <htracer/raytracing/image.hpp>
#include <htracer/raytracing/policies.hpp>
#include <htracer/raytracing/sampler.hpp>
#include <htracer/staging/scene_view.hpp>
#include <htracer/utils/randomness.hpp>
#include <htracer/vector.hpp>

#include <algorithm>
#include <cmath>
#include <execution>
#include <ranges>
#include <vector>


namespace htracer::raytracing::cameras
{

template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
class sampling;

template<
    typename Float,
    lens<Float> Lens,
    pixel_sampler<Float> PixelSampler,
    utils::uniform_random_generator Generator,
    template<typename>
    typename... Geometries>
class sampling_render_operation
{
  constexpr auto
  get_coords(uint32_t v_idx, uint32_t h_idx) const;

  constexpr auto
  get_ray(Float dh, Float dv) const;

  sampling<Float, Lens, PixelSampler> const &camera_;

  staging::scene_view<Float, Geometries...> scene_;
  uint32_t samples_;
  utils::randomness<Generator> &rand_;

public:
  sampling_render_operation(
      sampling<Float, Lens, PixelSampler> const &camera,
      staging::scene_view<Float, Geometries...> scene,
      uint32_t samples,
      utils::randomness<Generator> &rand)
      : camera_{camera}
      , scene_{scene}
      , samples_{samples}
      , rand_{rand}
  {
  }

  template<typename Policy>
  image<Float>
  operator()(Policy &&) const
  {
    using PolicyType = std::remove_cvref_t<Policy>;
    auto constexpr std_policy = policy_traits<PolicyType::style>::std_policy;

    auto const h_res = camera_.h_res();
    auto const v_res = camera_.v_res();
    auto const fov = camera_.fov();

    auto const v_tan = std::tan(fov);
    auto const h_tan = v_tan * h_res / v_res;

    auto const get_dv = [v_tan, v_res](Float idx)
    {
      return v_tan * (1 - ((2 * idx) / (v_res - 1)));
    };

    auto const get_dh = [h_tan, h_res](Float idx)
    {
      return h_tan * (((2 * idx) / (h_res - 1)) - 1);
    };

    uint32_t constexpr zero = 0;

    std::vector<colors::srgb_linear<Float>> pixels(v_res * h_res);
    auto v_indices = std::views::iota(zero, uint32_t{v_res});

    auto process_column = [&](auto const v_idx)
    {
      auto range = std::views::cartesian_product(std::views::iota(zero, h_res), std::views::iota(zero, samples_));

      for (auto const &[h_idx, sample_idx] : range)
      {
        auto const [v_coord, h_coord] = get_coords(v_idx, h_idx);
        auto const dv = get_dv(v_coord);
        auto const dh = get_dh(h_coord);

        auto const ray = get_ray(dv, dh);

        auto const scale = Float{1} / samples_;
        pixels[h_res * v_idx + h_idx] += sample(ray, scene_) * scale;
      }
    };

    // Jobs are batched by column.
    // Different approaches could be tested.
    // For example, do it for each pixel, for each sample, or even for each pixel and sample.
    // It is a tradeoff between contention and parallelization. It needs to be empirically tested.
    std::for_each(std_policy, std::begin(v_indices), std::end(v_indices), std::move(process_column));

    return {h_res, v_res, std::move(pixels)};
  }
};


template<
    typename Float,
    lens<Float> Lens,
    pixel_sampler<Float> PixelSampler,
    utils::uniform_random_generator Generator,
    template<typename>
    typename... Geometries>
constexpr auto
sampling_render_operation<Float, Lens, PixelSampler, Generator, Geometries...>::get_coords(
    uint32_t v_idx, uint32_t h_idx) const
{
  auto &pixel_sampler = camera_.pixel_sampler();

  if constexpr (deterministic_pixel_sampler<PixelSampler, Float>)
  {
    return pixel_sampler.get_coords(v_idx, h_idx);
  }
  else
  {
    return pixel_sampler.get_coords(v_idx, h_idx, rand_);
  }
}


template<
    typename Float,
    lens<Float> Lens,
    pixel_sampler<Float> PixelSampler,
    utils::uniform_random_generator Generator,
    template<typename>
    typename... Geometries>
constexpr auto
sampling_render_operation<Float, Lens, PixelSampler, Generator, Geometries...>::get_ray(Float dv, Float dh) const
{
  auto &lens = camera_.lens();

  auto const &position = camera_.position();
  auto const &view = camera_.view();
  auto const &up = camera_.up();
  auto const &right = camera_.right();

  if constexpr (deterministic_lens<Lens, Float>)
  {
    return lens.get_ray(dv, dh, position, view, up, right);
  }
  else
  {
    return lens.get_ray(dv, dh, position, view, up, right, rand_);
  }
}


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
class sampling
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
  sampling(
      v3<Float> const &position,
      v3<Float> const &view,
      v3<Float> const &up,
      uint32_t horizontal_resolution,
      uint32_t vertical_resolution,
      Float fov,
      Lens &lens,
      PixelSampler &pixel_sampler);

  constexpr v3<Float> const &
  position() const noexcept;
  constexpr v3<Float> const &
  view() const noexcept;
  constexpr v3<Float> const &
  up() const noexcept;
  constexpr v3<Float> const &
  right() const noexcept;
  constexpr uint32_t
  h_res() const noexcept;
  constexpr uint32_t
  v_res() const noexcept;
  constexpr Float
  fov() const noexcept;
  constexpr Lens &
  lens() const noexcept;
  constexpr PixelSampler &
  pixel_sampler() const noexcept;

  template<utils::uniform_random_generator Generator, template<typename> typename... Geometries>
  sampling_render_operation<Float, Lens, PixelSampler, Generator, Geometries...>
  render(
      staging::scene_view<Float, Geometries...> scene,
      uint32_t samples,
      utils::randomness<Generator> &rand) const;
};


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
sampling<Float, Lens, PixelSampler>::sampling(
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
constexpr v3<Float> const &
sampling<Float, Lens, PixelSampler>::position() const noexcept
{
  return position_;
}


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
constexpr v3<Float> const &
sampling<Float, Lens, PixelSampler>::view() const noexcept
{
  return view_;
}


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
constexpr v3<Float> const &
sampling<Float, Lens, PixelSampler>::up() const noexcept
{
  return up_;
}


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
constexpr v3<Float> const &
sampling<Float, Lens, PixelSampler>::right() const noexcept
{
  return right_;
}


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
constexpr uint32_t
sampling<Float, Lens, PixelSampler>::h_res() const noexcept
{
  return h_res_;
}


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
constexpr uint32_t
sampling<Float, Lens, PixelSampler>::v_res() const noexcept
{
  return v_res_;
}


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
constexpr Float
sampling<Float, Lens, PixelSampler>::fov() const noexcept
{
  return fov_;
}


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
constexpr Lens &
sampling<Float, Lens, PixelSampler>::lens() const noexcept
{
  return lens_;
}


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
constexpr PixelSampler &
sampling<Float, Lens, PixelSampler>::pixel_sampler() const noexcept
{
  return pixel_sampler_;
}


template<typename Float, lens<Float> Lens, pixel_sampler<Float> PixelSampler>
template<utils::uniform_random_generator Generator, template<typename> typename... Geometries>
sampling_render_operation<Float, Lens, PixelSampler, Generator, Geometries...>
sampling<Float, Lens, PixelSampler>::render(
    staging::scene_view<Float, Geometries...> scene,
    uint32_t samples,
    utils::randomness<Generator> &rand) const
{
  return sampling_render_operation(*this, scene, samples, rand);
}

} // namespace htracer::raytracing::cameras

#endif
