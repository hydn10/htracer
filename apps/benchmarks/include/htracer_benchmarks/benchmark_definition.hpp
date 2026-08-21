#ifndef HTRACER_BENCHMARKS_BENCHMARK_DEFINITION_HPP
#define HTRACER_BENCHMARKS_BENCHMARK_DEFINITION_HPP


#include <htracer_benchmarks/image_extent.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>
#include <htracer_benchmarks/render_mode.hpp>
#include <htracer_benchmarks/scene_spec.hpp>


namespace htracer::benchmarks
{

enum class precision_kind : std::uint8_t
{
  f32,
  f64,
};


enum class policy_kind : std::uint8_t
{
  seq,
  par,
};


class benchmark_definition
{
  scene_spec scene_;
  render_mode rendering_;
  precision_kind precision_;
  policy_kind policy_;
  image_extent extent_;
  measurement_plan measurement_;

  benchmark_definition(
      scene_spec scene,
      render_mode rendering,
      precision_kind precision,
      policy_kind policy,
      image_extent extent,
      measurement_plan measurement);

public:
  [[nodiscard]]
  static benchmark_definition
  deterministic(
      deterministic_scene_spec scene,
      precision_kind precision,
      policy_kind policy,
      image_extent extent,
      measurement_plan measurement);

  [[nodiscard]]
  static benchmark_definition
  randomized(
      scene_spec scene,
      randomized_render rendering,
      precision_kind precision,
      policy_kind policy,
      image_extent extent,
      measurement_plan measurement);

  [[nodiscard]]
  constexpr scene_spec const &
  scene() const noexcept
  {
    return scene_;
  }

  [[nodiscard]]
  constexpr render_mode const &
  rendering() const noexcept
  {
    return rendering_;
  }

  [[nodiscard]]
  constexpr precision_kind
  precision() const noexcept
  {
    return precision_;
  }

  [[nodiscard]]
  constexpr policy_kind
  policy() const noexcept
  {
    return policy_;
  }

  [[nodiscard]]
  constexpr image_extent
  extent() const noexcept
  {
    return extent_;
  }

  [[nodiscard]]
  constexpr measurement_plan
  measurement() const noexcept
  {
    return measurement_;
  }
};

} // namespace htracer::benchmarks

#endif
