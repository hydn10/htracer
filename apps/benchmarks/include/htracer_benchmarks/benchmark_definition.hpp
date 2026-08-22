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
  scene() const noexcept;

  [[nodiscard]]
  constexpr render_mode const &
  rendering() const noexcept;

  [[nodiscard]]
  constexpr precision_kind
  precision() const noexcept;

  [[nodiscard]]
  constexpr policy_kind
  policy() const noexcept;

  [[nodiscard]]
  constexpr image_extent
  extent() const noexcept;

  [[nodiscard]]
  constexpr measurement_plan
  measurement() const noexcept;
};


constexpr scene_spec const &
benchmark_definition::scene() const noexcept
{
  return scene_;
}


constexpr render_mode const &
benchmark_definition::rendering() const noexcept
{
  return rendering_;
}


constexpr precision_kind
benchmark_definition::precision() const noexcept
{
  return precision_;
}


constexpr policy_kind
benchmark_definition::policy() const noexcept
{
  return policy_;
}


constexpr image_extent
benchmark_definition::extent() const noexcept
{
  return extent_;
}


constexpr measurement_plan
benchmark_definition::measurement() const noexcept
{
  return measurement_;
}

} // namespace htracer::benchmarks

#endif
