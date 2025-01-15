#ifndef HTRACER_RAYTRACING_CAMERAS_SAMPLING_HPP
#define HTRACER_RAYTRACING_CAMERAS_SAMPLING_HPP


#include <htracer/raytracing/concepts.hpp>
#include <htracer/raytracing/image.hpp>
#include <htracer/raytracing/policies.hpp>
#include <htracer/raytracing/sampler.hpp>
#include <htracer/scene/scene_view.hpp>
#include <htracer/utils/randomness.hpp>
#include <htracer/vector.hpp>

#include <algorithm>
#include <cmath>
#include <execution>
#include <numeric>
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
  template<policy_type PolicyType>
  struct policy_traits
  {
    static auto constexpr good = false;
  };

  template<>
  struct policy_traits<policy_type::UNSEQUENCED>
  {
    static auto constexpr good = true;
    static auto constexpr std_policy = std::execution::unseq;
  };

  template<>
  struct policy_traits<policy_type::PARALLEL>
  {
    static auto constexpr good = true;
    static auto constexpr std_policy = std::execution::par_unseq;
  };

  static auto
  get_coords(PixelSampler &pixel_sampler, uint32_t v_idx, uint32_t h_idx, utils::randomness<Generator> &rand);

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

  sampling<Float, Lens, PixelSampler> const &camera_;

  scene::scene_view<scene::scene<Float, Geometries...>> scene_;
  uint32_t samples_;
  utils::randomness<Generator> &rand_;

public:
  sampling_render_operation(
      sampling<Float, Lens, PixelSampler> const &camera,
      scene::scene_view<scene::scene<Float, Geometries...>> scene,
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
    auto constexpr std_policy = policy_traits<PolicyType::policy_type>::std_policy;

    auto const &position = camera_.position();
    auto const &view = camera_.view();
    auto const &up = camera_.up();
    auto const &right = camera_.right();
    auto const h_res = camera_.h_res();
    auto const v_res = camera_.v_res();
    auto const fov = camera_.fov();
    auto const &lens = camera_.lens();
    auto const &pixel_sampler = camera_.pixel_sampler();

    auto const v_tan = std::tan(fov);
    auto const h_tan = v_tan * h_res / v_res;

    std::vector<colors::srgb_linear<Float>> pixels(v_res * h_res);

    std::vector<uint32_t> v_indices(v_res);
    std::iota(std::begin(v_indices), std::end(v_indices), 0);

    auto get_dv = [v_tan, v_res = v_res](Float idx)
    {
      return v_tan * (1 - ((2 * idx) / (v_res - 1)));
    };

    auto get_dh = [h_tan, h_res = h_res](Float idx)
    {
      return h_tan * (((2 * idx) / (h_res - 1)) - 1);
    };

    std::for_each(
        std_policy,
        std::begin(v_indices),
        std::end(v_indices),
        [&](auto const i)
        {
          for (uint32_t j = 0; j < h_res; ++j)
          {
            for (auto k = samples_; k > 0; --k)
            {
              auto const [v_coord, h_coord] = get_coords(pixel_sampler, i, j, rand_);
              auto const dv = get_dv(v_coord);
              auto const dh = get_dh(h_coord);

              auto const ray = get_ray(lens, dv, dh, position, view, up, right, rand_);

              pixels[h_res * i + j] += sample(ray, scene_);
            }
          }
        });

    for (auto &pixel : pixels)
    {
      auto const scale = Float{1} / samples_;
      pixel *= scale;
    }

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
auto
sampling_render_operation<Float, Lens, PixelSampler, Generator, Geometries...>::get_coords(
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


template<
    typename Float,
    lens<Float> Lens,
    pixel_sampler<Float> PixelSampler,
    utils::uniform_random_generator Generator,
    template<typename>
    typename... Geometries>
auto
sampling_render_operation<Float, Lens, PixelSampler, Generator, Geometries...>::get_ray(
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
  position() const noexcept
  {
    return position_;
  }
  constexpr v3<Float> const &
  view() const noexcept
  {
    return view_;
  }
  constexpr v3<Float> const &
  up() const noexcept
  {
    return up_;
  }
  constexpr v3<Float> const &
  right() const noexcept
  {
    return right_;
  }
  constexpr uint32_t
  h_res() const noexcept
  {
    return h_res_;
  }
  constexpr uint32_t
  v_res() const noexcept
  {
    return v_res_;
  }
  constexpr Float
  fov() const noexcept
  {
    return fov_;
  }
  constexpr Lens &
  lens() const noexcept
  {
    return lens_;
  }
  constexpr PixelSampler &
  pixel_sampler() const noexcept
  {
    return pixel_sampler_;
  }

  template<utils::uniform_random_generator Generator, template<typename> typename... Geometries>
  sampling_render_operation<Float, Lens, PixelSampler, Generator, Geometries...>
  render(
      scene::scene_view<scene::scene<Float, Geometries...>> scene,
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
template<utils::uniform_random_generator Generator, template<typename> typename... Geometries>
sampling_render_operation<Float, Lens, PixelSampler, Generator, Geometries...>
sampling<Float, Lens, PixelSampler>::render(
    scene::scene_view<scene::scene<Float, Geometries...>> scene,
    uint32_t samples,
    utils::randomness<Generator> &rand) const
{
  return sampling_render_operation(*this, scene, samples, rand);
}

} // namespace htracer::raytracing::cameras

#endif
