#ifndef HTRACER_BENCHMARKS_RENDER_MODE_HPP
#define HTRACER_BENCHMARKS_RENDER_MODE_HPP


#include <htracer/rendering/random_seed.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>

#include <optional>
#include <utility>
#include <variant>


namespace htracer::benchmarks
{

struct deterministic_render
{
};


class randomized_render
{
  htracer::rendering::samples_per_pixel samples_;
  std::optional<htracer::rendering::random_seed> seed_;

public:
  constexpr randomized_render(
      htracer::rendering::samples_per_pixel samples, std::optional<htracer::rendering::random_seed> seed) noexcept
      : samples_{samples}
      , seed_{seed}
  {
  }

  [[nodiscard]]
  constexpr htracer::rendering::samples_per_pixel
  samples() const noexcept
  {
    return samples_;
  }

  [[nodiscard]]
  constexpr std::optional<htracer::rendering::random_seed>
  seed() const noexcept
  {
    return seed_;
  }
};


using render_mode = std::variant<deterministic_render, randomized_render>;

} // namespace htracer::benchmarks

#endif
