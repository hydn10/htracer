#include <htracer_benchmarks/cli.hpp>

#include <htracer/rendering/random_seed.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>
#include <htracer_benchmarks/model.hpp>

#include <charconv>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <print>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <variant>


namespace htracer::benchmarks
{

namespace
{

struct raw_options
{
  bool help{};
  bool list{};
  std::optional<std::string> suite;
  std::optional<std::string> benchmark;
  std::optional<std::string> scene;
  std::optional<std::string> rendering;
  std::optional<std::string> width;
  std::optional<std::string> height;
  std::optional<std::string> geometry_count;
  std::optional<std::string> samples;
  std::optional<std::string> precision;
  std::optional<std::string> policy;
  std::optional<std::string> seed;
  std::optional<std::string> warmup;
  std::optional<std::string> repetitions;
  std::optional<std::string> output;
};


template<typename T>
void
set_once(std::optional<T> &destination, T value, std::string_view option)
{
  if (destination)
  {
    throw usage_error("duplicate option: " + std::string{option});
  }
  destination = std::move(value);
}


void
set_flag_once(bool &destination, std::string_view option)
{
  if (destination)
  {
    throw usage_error("duplicate option: " + std::string{option});
  }
  destination = true;
}


[[nodiscard]]
std::string
take_value(std::size_t &index, std::span<char const *const> arguments, std::string_view option)
{
  if (index + 1 >= arguments.size())
  {
    throw usage_error("missing value for option: " + std::string{option});
  }
  std::string value{arguments[++index]};
  if (value.starts_with("--"))
  {
    throw usage_error("missing value for option: " + std::string{option});
  }
  return value;
}


[[nodiscard]]
raw_options
parse_raw_options(std::span<char const *const> arguments)
{
  raw_options options;
  for (std::size_t index = 0; index < arguments.size(); ++index)
  {
    std::string_view const option{arguments[index]};
    if (option == "--help")
    {
      set_flag_once(options.help, option);
    }
    else if (option == "--list")
    {
      set_flag_once(options.list, option);
    }
    else if (option == "--suite")
    {
      set_once(options.suite, take_value(index, arguments, option), option);
    }
    else if (option == "--benchmark")
    {
      set_once(options.benchmark, take_value(index, arguments, option), option);
    }
    else if (option == "--scene")
    {
      set_once(options.scene, take_value(index, arguments, option), option);
    }
    else if (option == "--rendering")
    {
      set_once(options.rendering, take_value(index, arguments, option), option);
    }
    else if (option == "--width")
    {
      set_once(options.width, take_value(index, arguments, option), option);
    }
    else if (option == "--height")
    {
      set_once(options.height, take_value(index, arguments, option), option);
    }
    else if (option == "--geometry-count")
    {
      set_once(options.geometry_count, take_value(index, arguments, option), option);
    }
    else if (option == "--samples")
    {
      set_once(options.samples, take_value(index, arguments, option), option);
    }
    else if (option == "--precision")
    {
      set_once(options.precision, take_value(index, arguments, option), option);
    }
    else if (option == "--policy")
    {
      set_once(options.policy, take_value(index, arguments, option), option);
    }
    else if (option == "--seed")
    {
      set_once(options.seed, take_value(index, arguments, option), option);
    }
    else if (option == "--warmup")
    {
      set_once(options.warmup, take_value(index, arguments, option), option);
    }
    else if (option == "--repetitions")
    {
      set_once(options.repetitions, take_value(index, arguments, option), option);
    }
    else if (option == "--output")
    {
      set_once(options.output, take_value(index, arguments, option), option);
    }
    else
    {
      throw usage_error("unknown option: " + std::string{option});
    }
  }
  return options;
}


template<typename UInt>
requires std::is_unsigned_v<UInt>
[[nodiscard]]
UInt
parse_unsigned(std::string const &text, std::string_view option)
{
  UInt value{};
  auto const *const begin = text.data();
  auto const *const finish = begin + text.size(); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  auto const [end, error] = std::from_chars(begin, finish, value);
  if (error != std::errc{} || end != finish)
  {
    throw usage_error("invalid unsigned integer for " + std::string{option} + ": " + text);
  }
  return value;
}


void
require(bool condition, std::string const &message)
{
  if (!condition)
  {
    throw usage_error(message);
  }
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


[[nodiscard]]
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


[[nodiscard]]
bool
has_custom_options(raw_options const &options)
{
  return options.scene || options.rendering || options.width || options.height || options.geometry_count ||
         options.samples || options.precision || options.policy || options.seed || options.warmup ||
         options.repetitions;
}

} // namespace


invocation
parse_cli(std::span<char const *const> arguments)
{
  auto const options = parse_raw_options(arguments);
  auto const operation_count = static_cast<unsigned>(options.help) + static_cast<unsigned>(options.list) +
                               static_cast<unsigned>(options.suite.has_value()) +
                               static_cast<unsigned>(options.benchmark.has_value());
  require(operation_count == 1, "specify exactly one of --help, --list, --suite, or --benchmark");

  if (options.help)
  {
    require(!options.output && !has_custom_options(options), "--help does not accept additional options");
    return help_command{};
  }
  if (options.list)
  {
    require(!options.output && !has_custom_options(options), "--list does not accept additional options");
    return list_command{};
  }

  auto const output = options.output ? std::optional{std::filesystem::path{*options.output}} : std::nullopt;
  if (options.suite)
  {
    require(*options.suite == "quick", "--suite only supports: quick");
    require(!has_custom_options(options), "--suite quick only accepts --output");
    return quick_suite_command{.output = output};
  }

  return custom_render_command{.benchmark = parse_custom_definition(options), .output = output};
}


void
print_help()
{
  std::print(
      "Usage:\n"
      "  htracer-benchmarks --help\n"
      "  htracer-benchmarks --list\n"
      "  htracer-benchmarks --suite quick [--output PATH]\n"
      "  htracer-benchmarks --benchmark render OPTIONS\n\n"
      "Required custom render options:\n"
      "  --scene mixed|traversal|rng-probe\n"
      "  --rendering deterministic|randomized\n"
      "  --width N --height N\n"
      "  --precision float|double\n"
      "  --policy seq|par\n\n"
      "Conditional and measurement options:\n"
      "  --geometry-count N  Required only for traversal\n"
      "  --samples N         Required only for randomized rendering\n"
      "  --seed none|N       Optional unsigned decimal seed for randomized rendering\n"
      "  --warmup N          Default: 1; zero is allowed\n"
      "  --repetitions N     Default: 9\n"
      "  --output PATH       Write versioned JSON in addition to console output\n");
}

} // namespace htracer::benchmarks
