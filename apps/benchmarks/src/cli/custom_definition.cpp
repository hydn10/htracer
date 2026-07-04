#include <htracer_benchmarks/cli/custom_definition.hpp>

#include <htracer/rendering/random_seed.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/cli.hpp>
#include <htracer_benchmarks/cli/raw_options.hpp>
#include <htracer_benchmarks/image_extent.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>
#include <htracer_benchmarks/render_mode.hpp>
#include <htracer_benchmarks/scene_spec.hpp>

#include <concepts>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>


namespace htracer::benchmarks::cli
{

namespace
{

[[nodiscard]]
precision_kind
parse_precision(std::string const &value)
{
  if (value == "float")
  {
    return precision_kind::f32;
  }
  if (value == "double")
  {
    return precision_kind::f64;
  }
  throw usage_error("invalid --precision value: " + value);
}


[[nodiscard]]
policy_kind
parse_policy(std::string const &value)
{
  if (value == "seq")
  {
    return policy_kind::seq;
  }
  if (value == "par")
  {
    return policy_kind::par;
  }
  throw usage_error("invalid --policy value: " + value);
}


template<typename Factory>
[[nodiscard]]
auto
domain_value(Factory &&factory)
{
  try
  {
    return std::forward<Factory>(factory)();
  }
  catch (std::invalid_argument const &error)
  {
    throw usage_error(error.what());
  }
}


[[nodiscard]]
scene_spec
parse_scene(raw_options const &options)
{
  if (*options.scene == "mixed")
  {
    require(!options.geometry_count, "--geometry-count is only valid for the traversal scene");
    return mixed_scene{};
  }
  if (*options.scene == "rng-probe")
  {
    require(!options.geometry_count, "--geometry-count is only valid for the traversal scene");
    return rng_probe_scene{};
  }
  if (*options.scene == "traversal")
  {
    require(options.geometry_count.has_value(), "traversal scene requires --geometry-count");
    auto const count = parse_unsigned<std::uint32_t>(*options.geometry_count, "--geometry-count");
    return traversal_scene{domain_value([count] { return geometry_count::make(count); })};
  }
  throw usage_error("invalid --scene value: " + *options.scene);
}


[[nodiscard]]
render_mode
parse_rendering(raw_options const &options)
{
  if (*options.rendering == "deterministic")
  {
    require(!options.samples, "--samples is invalid for deterministic rendering");
    require(!options.seed, "--seed is invalid for deterministic rendering");
    return deterministic_render{};
  }
  if (*options.rendering == "randomized")
  {
    require(options.samples.has_value(), "randomized rendering requires --samples");
    auto const samples = parse_unsigned<std::uint32_t>(*options.samples, "--samples");
    std::optional<htracer::rendering::random_seed> seed;
    if (options.seed && *options.seed != "none")
    {
      seed = htracer::rendering::random_seed{parse_unsigned<std::uint64_t>(*options.seed, "--seed")};
    }
    return domain_value([samples, seed]
    { return randomized_render::make(htracer::rendering::samples_per_pixel{samples}, seed); });
  }
  throw usage_error("invalid --rendering value: " + *options.rendering);
}

} // namespace


benchmark_definition
parse_custom_definition(raw_options const &options)
{
  require(options.benchmark && *options.benchmark == "render", "--benchmark only supports: render");
  require(options.scene.has_value(), "custom render requires --scene");
  require(options.rendering.has_value(), "custom render requires --rendering");
  require(options.width.has_value(), "custom render requires --width");
  require(options.height.has_value(), "custom render requires --height");
  require(options.precision.has_value(), "custom render requires --precision");
  require(options.policy.has_value(), "custom render requires --policy");

  auto const width = parse_unsigned<std::uint32_t>(*options.width, "--width");
  auto const height = parse_unsigned<std::uint32_t>(*options.height, "--height");
  auto const warmups = options.warmup ? parse_unsigned<std::uint32_t>(*options.warmup, "--warmup") : 1U;
  auto const repetitions =
      options.repetitions ? parse_unsigned<std::uint32_t>(*options.repetitions, "--repetitions") : 9U;

  auto scene = parse_scene(options);
  auto rendering = parse_rendering(options);
  auto const precision = parse_precision(*options.precision);
  auto const policy = parse_policy(*options.policy);
  auto const extent = domain_value([width, height] { return image_extent::make(width, height); });
  auto const measurement = measurement_plan{
      .warmups = warmup_count{warmups},
      .repetitions = domain_value([repetitions] { return repetition_count::make(repetitions); })};

  return std::visit(
      [&]<typename Mode>([[maybe_unused]]
                         Mode mode)
  {
    if constexpr (std::same_as<Mode, deterministic_render>)
    {
      return domain_value([&]
      { return benchmark_definition::deterministic(scene, precision, policy, extent, measurement); });
    }
    else
    {
      static_assert(std::same_as<Mode, randomized_render>);
      return domain_value([&]
      { return benchmark_definition::randomized(scene, mode, precision, policy, extent, measurement); });
    }
  },
      rendering);
}

} // namespace htracer::benchmarks::cli
