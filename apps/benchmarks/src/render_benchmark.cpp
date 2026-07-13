#include <htracer_benchmarks/render_benchmark.hpp>

#include <htracer/htracer.hpp>
#include <htracer_benchmarks/benchmark_case.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/benchmark_result.hpp>
#include <htracer_benchmarks/execution/measurement.hpp>
#include <htracer_benchmarks/image_extent.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>
#include <htracer_benchmarks/render_mode.hpp>
#include <htracer_benchmarks/scenes/make_scene.hpp>

#include <concepts>
#include <utility>
#include <variant>


namespace htracer::benchmarks
{

namespace
{

template<std::floating_point Float, typename Renderer, typename Scene, htracer::rendering::rendering_policy Policy>
[[nodiscard]]
benchmark_result
measure_deterministic(
    benchmark_case const &benchmark,
    image_extent extent,
    measurement_plan plan,
    Renderer const &renderer,
    Scene const &scene,
    Policy policy)
{
  return execution::measure<Float>(
      benchmark, extent, plan, true, [&renderer, &scene, policy]() { return renderer.render(policy, scene); });
}


template<std::floating_point Float, typename Renderer, typename Scene, htracer::rendering::rendering_policy Policy>
[[nodiscard]]
benchmark_result
measure_randomized(
    benchmark_case const &benchmark,
    image_extent extent,
    measurement_plan plan,
    randomized_render rendering,
    Renderer const &renderer,
    Scene const &scene,
    Policy policy)
{
  auto const seed = rendering.seed();
  if (seed)
  {
    auto const seed_value = *seed;
    return execution::measure<Float>(
        benchmark, extent, plan, true, [&renderer, &scene, policy, rendering, seed_value]() {
      return renderer.render(policy, scene, rendering.samples(), seed_value);
    });
  }
  return execution::measure<Float>(benchmark, extent, plan, false, [&renderer, &scene, policy, rendering]() {
    return renderer.render(policy, scene, rendering.samples());
  });
}


template<std::floating_point Float, typename Renderer, typename Scene, typename Mode>
[[nodiscard]]
benchmark_result
dispatch_policy(
    benchmark_case const &benchmark,
    policy_kind execution_policy,
    image_extent extent,
    measurement_plan plan,
    Mode mode,
    Renderer const &renderer,
    Scene const &scene)
{
  switch (execution_policy)
  {
  case policy_kind::seq:
    if constexpr (std::same_as<Mode, deterministic_render>)
    {
      return measure_deterministic<Float>(benchmark, extent, plan, renderer, scene, htracer::rendering::seq);
    }
    else
    {
      return measure_randomized<Float>(benchmark, extent, plan, mode, renderer, scene, htracer::rendering::seq);
    }
  case policy_kind::par:
    if constexpr (std::same_as<Mode, deterministic_render>)
    {
      return measure_deterministic<Float>(benchmark, extent, plan, renderer, scene, htracer::rendering::par);
    }
    else
    {
      return measure_randomized<Float>(benchmark, extent, plan, mode, renderer, scene, htracer::rendering::par);
    }
  }
  std::unreachable();
}


template<std::floating_point Float>
[[nodiscard]]
benchmark_result
run_typed(benchmark_case const &benchmark, benchmark_definition const &definition)
{
  using traits = htracer::float_traits<Float>;

  auto setup = scenes::make_scene<Float>(definition.scene());
  auto const extent = definition.extent();
  typename traits::camera const camera{
      setup.camera_position, setup.camera_view, setup.camera_up, extent.width(), extent.height(), setup.fov};
  htracer::rendering::batchers::column_batcher const batcher;
  typename traits::pinhole_lens const lens;

  return std::visit(
      [&]<typename Mode>(Mode mode) -> benchmark_result
  {
    if constexpr (std::same_as<Mode, deterministic_render>)
    {
      typename traits::point_sensor const sensor;
      auto const renderer = htracer::rendering::make_renderer(camera, batcher, sensor, lens);
      return dispatch_policy<Float>(
          benchmark, definition.policy(), definition.extent(), definition.measurement(), mode, renderer, setup.scene);
    }
    else
    {
      static_assert(std::same_as<Mode, randomized_render>);
      typename traits::uniform_sensor const sensor;
      auto const renderer = htracer::rendering::make_renderer(camera, batcher, sensor, lens);
      return dispatch_policy<Float>(
          benchmark, definition.policy(), definition.extent(), definition.measurement(), mode, renderer, setup.scene);
    }
  },
      definition.rendering());
}


[[nodiscard]]
benchmark_result
run_definition(benchmark_case const &benchmark, benchmark_definition const &definition)
{
  switch (definition.precision())
  {
  case precision_kind::f32:
    return run_typed<float>(benchmark, definition);
  case precision_kind::f64:
    return run_typed<double>(benchmark, definition);
  }
  std::unreachable();
}

} // namespace


benchmark_result
run_benchmark(benchmark_case const &benchmark)
{
  return run_definition(benchmark, benchmark.definition());
}

} // namespace htracer::benchmarks
