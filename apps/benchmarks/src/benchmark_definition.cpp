#include <htracer_benchmarks/benchmark_definition.hpp>

#include <htracer_benchmarks/image_extent.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>
#include <htracer_benchmarks/render_mode.hpp>
#include <htracer_benchmarks/scene_spec.hpp>

#include <stdexcept>
#include <variant>


namespace htracer::benchmarks
{

benchmark_definition::benchmark_definition(
    scene_spec scene,
    render_mode rendering,
    precision_kind precision,
    policy_kind policy,
    image_extent extent,
    measurement_plan measurement)
    : scene_{scene}
    , rendering_{rendering}
    , precision_{precision}
    , policy_{policy}
    , extent_{extent}
    , measurement_{measurement}
{
}


benchmark_definition
benchmark_definition::deterministic(
    scene_spec scene, precision_kind precision, policy_kind policy, image_extent extent, measurement_plan measurement)
{
  if (std::holds_alternative<rng_probe_scene>(scene))
  {
    throw std::invalid_argument("rng-probe scene requires randomized rendering");
  }
  return {scene, deterministic_render{}, precision, policy, extent, measurement};
}


benchmark_definition
benchmark_definition::randomized(
    scene_spec scene,
    randomized_render rendering,
    precision_kind precision,
    policy_kind policy,
    image_extent extent,
    measurement_plan measurement)
{
  return {scene, rendering, precision, policy, extent, measurement};
}

} // namespace htracer::benchmarks
