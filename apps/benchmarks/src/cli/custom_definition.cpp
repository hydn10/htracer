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
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>


namespace htracer::benchmarks::cli
{

namespace
{

template<typename T>
[[nodiscard]]
T const &
required_value(std::optional<T> const &value, std::string_view message)
{
  if (!value)
  {
    throw usage_error(std::string{message});
  }
  return *value;
}


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
parse_scene(std::string const &scene, std::optional<std::string> const &geometry_count_option)
{
  if (scene == "mixed")
  {
    require(!geometry_count_option, "--geometry-count is only valid for the traversal scene");
    return mixed_scene{};
  }
  if (scene == "rng-probe")
  {
    require(!geometry_count_option, "--geometry-count is only valid for the traversal scene");
    return rng_probe_scene{};
  }
  if (scene == "traversal")
  {
    auto const &geometry_count = required_value(geometry_count_option, "traversal scene requires --geometry-count");
    auto const count = parse_unsigned<std::uint32_t>(geometry_count, "--geometry-count");
    return traversal_scene{domain_value([count] { return geometry_count::make(count); })};
  }
  throw usage_error("invalid --scene value: " + scene);
}


[[nodiscard]]
render_mode
parse_rendering(
    std::string const &rendering,
    std::optional<std::string> const &samples_option,
    std::optional<std::string> const &seed_option)
{
  if (rendering == "deterministic")
  {
    require(!samples_option, "--samples is invalid for deterministic rendering");
    require(!seed_option, "--seed is invalid for deterministic rendering");
    return deterministic_render{};
  }
  if (rendering == "randomized")
  {
    auto const &samples_text = required_value(samples_option, "randomized rendering requires --samples");
    auto const samples = parse_unsigned<std::uint32_t>(samples_text, "--samples");
    std::optional<htracer::rendering::random_seed> seed;
    if (seed_option && *seed_option != "none")
    {
      seed = htracer::rendering::random_seed{parse_unsigned<std::uint64_t>(*seed_option, "--seed")};
    }
    return domain_value([samples, seed]
    { return randomized_render{htracer::rendering::samples_per_pixel{samples}, seed}; });
  }
  throw usage_error("invalid --rendering value: " + rendering);
}

} // namespace


benchmark_definition
parse_custom_definition(raw_options const &options)
{
  auto const &benchmark = required_value(options.benchmark, "custom render requires --benchmark");
  require(benchmark == "render", "--benchmark only supports: render");
  auto const &scene_text = required_value(options.scene, "custom render requires --scene");
  auto const &rendering_text = required_value(options.rendering, "custom render requires --rendering");
  auto const &width_text = required_value(options.width, "custom render requires --width");
  auto const &height_text = required_value(options.height, "custom render requires --height");
  auto const &precision_text = required_value(options.precision, "custom render requires --precision");
  auto const &policy_text = required_value(options.policy, "custom render requires --policy");

  auto const width = parse_unsigned<std::uint32_t>(width_text, "--width");
  auto const height = parse_unsigned<std::uint32_t>(height_text, "--height");
  auto const warmups = options.warmup ? parse_unsigned<std::uint32_t>(*options.warmup, "--warmup") : 1U;
  auto const repetitions =
      options.repetitions ? parse_unsigned<std::uint32_t>(*options.repetitions, "--repetitions") : 9U;

  auto scene = parse_scene(scene_text, options.geometry_count);
  auto rendering = parse_rendering(rendering_text, options.samples, options.seed);
  auto const precision = parse_precision(precision_text);
  auto const policy = parse_policy(policy_text);
  auto const parsed_width = domain_value([width] { return image_width::make(width); });
  auto const parsed_height = domain_value([height] { return image_height::make(height); });
  auto const extent =
      domain_value([parsed_width, parsed_height] { return image_extent::make(parsed_width, parsed_height); });
  auto const measurement = measurement_plan{
      .warmups = warmup_count{warmups},
      .repetitions = domain_value([repetitions] { return repetition_count::make(repetitions); })};

  return std::visit(
      [&]<typename Mode>([[maybe_unused]]
                         Mode mode)
  {
    if constexpr (std::same_as<Mode, deterministic_render>)
    {
      return std::visit(
          [&](auto const &scene_value) -> benchmark_definition
      {
        using Scene = std::remove_cvref_t<decltype(scene_value)>;
        if constexpr (std::same_as<Scene, rng_probe_scene>)
        {
          throw usage_error("rng-probe scene requires randomized rendering");
        }
        else
        {
          return benchmark_definition::deterministic(
              deterministic_scene_spec{scene_value}, precision, policy, extent, measurement);
        }
      },
          scene);
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
