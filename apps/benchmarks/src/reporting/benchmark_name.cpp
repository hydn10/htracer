#include <htracer_benchmarks/reporting/benchmark_name.hpp>

#include <htracer_benchmarks/benchmark_case.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/render_mode.hpp>
#include <htracer_benchmarks/scene_spec.hpp>
#include <htracer_benchmarks/schema_version.hpp>

#include <concepts>
#include <format>
#include <string>
#include <string_view>
#include <utility>
#include <variant>


namespace htracer::benchmarks::reporting
{

namespace
{

[[nodiscard]]
std::string_view
precision_id(precision_kind precision) noexcept
{
  switch (precision)
  {
  case precision_kind::f32:
    return "f32";
  case precision_kind::f64:
    return "f64";
  }
  std::unreachable();
}


[[nodiscard]]
std::string
scene_id(scene_spec const &scene)
{
  return std::visit(
      []<typename Scene>(Scene const &value) -> std::string
      {
        if constexpr (std::same_as<Scene, mixed_scene>)
        {
          return "mixed";
        }
        else if constexpr (std::same_as<Scene, traversal_scene>)
        {
          return std::format("traversal-g{}", value.count.value());
        }
        else
        {
          static_assert(std::same_as<Scene, rng_probe_scene>);
          return "rng-probe";
        }
      },
      scene);
}


[[nodiscard]]
std::string
rendering_id(render_mode const &rendering)
{
  return std::visit(
      []<typename Mode>(Mode const &mode) -> std::string
      {
        if constexpr (std::same_as<Mode, deterministic_render>)
        {
          return "deterministic";
        }
        else
        {
          static_assert(std::same_as<Mode, randomized_render>);
          auto const seed = mode.seed();
          auto const seed_value = seed ? std::format("{:016x}", seed->value) : std::string{"none"};
          return std::format("randomized-spp{}-seed{}", mode.samples().value(), seed_value);
        }
      },
      rendering);
}


[[nodiscard]]
std::string
canonical_benchmark_name(benchmark_definition const &definition)
{
  auto const extent = definition.extent();
  auto const measurement = definition.measurement();
  return std::format(
      "quick.v{}/{}/{}/{}/{}/{}x{}/w{}/r{}",
      workload_schema_version,
      scene_id(definition.scene()),
      rendering_id(definition.rendering()),
      precision_id(definition.precision()),
      policy_name(definition.policy()),
      extent.width().value(),
      extent.height().value(),
      measurement.warmups.value(),
      measurement.repetitions.value());
}

} // namespace


std::string_view
scene_name(scene_spec const &scene)
{
  return std::visit(
      []<typename Scene>(Scene const &) -> std::string_view
      {
        if constexpr (std::same_as<Scene, mixed_scene>)
        {
          return "mixed";
        }
        else if constexpr (std::same_as<Scene, traversal_scene>)
        {
          return "traversal";
        }
        else
        {
          static_assert(std::same_as<Scene, rng_probe_scene>);
          return "rng-probe";
        }
      },
      scene);
}


std::string_view
rendering_name(render_mode const &rendering)
{
  return std::visit(
      []<typename Mode>(Mode const &) -> std::string_view
      {
        if constexpr (std::same_as<Mode, deterministic_render>)
        {
          return "deterministic";
        }
        else
        {
          static_assert(std::same_as<Mode, randomized_render>);
          return "randomized";
        }
      },
      rendering);
}


std::string_view
precision_name(precision_kind precision) noexcept
{
  switch (precision)
  {
  case precision_kind::f32:
    return "float";
  case precision_kind::f64:
    return "double";
  }
  std::unreachable();
}


std::string_view
policy_name(policy_kind policy) noexcept
{
  switch (policy)
  {
  case policy_kind::seq:
    return "seq";
  case policy_kind::par:
    return "par";
  }
  std::unreachable();
}


std::string
benchmark_name(benchmark_case const &benchmark)
{
  if (!benchmark.is_canonical())
  {
    return "custom/render";
  }
  return canonical_benchmark_name(benchmark.definition());
}

} // namespace htracer::benchmarks::reporting
