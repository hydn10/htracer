#include <htracer_benchmarks/cli.hpp>
#include <htracer_benchmarks/model.hpp>

#include <charconv>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <limits>
#include <optional>
#include <ostream>
#include <span>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <utility>


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


[[nodiscard]]
scene_kind
parse_scene(std::string const &value)
{
  if (value == "mixed")
  {
    return scene_kind::mixed;
  }
  if (value == "traversal")
  {
    return scene_kind::traversal;
  }
  if (value == "rng-probe")
  {
    return scene_kind::rng_probe;
  }
  throw usage_error("invalid --scene value: " + value);
}


[[nodiscard]]
rendering_kind
parse_rendering(std::string const &value)
{
  if (value == "deterministic")
  {
    return rendering_kind::deterministic;
  }
  if (value == "randomized")
  {
    return rendering_kind::randomized;
  }
  throw usage_error("invalid --rendering value: " + value);
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


void
require(bool condition, std::string const &message)
{
  if (!condition)
  {
    throw usage_error(message);
  }
}


[[nodiscard]]
benchmark_case
make_custom_case(raw_options const &options)
{
  require(options.benchmark && *options.benchmark == "render", "--benchmark only supports: render");
  require(options.scene.has_value(), "custom render requires --scene");
  require(options.rendering.has_value(), "custom render requires --rendering");
  require(options.width.has_value(), "custom render requires --width");
  require(options.height.has_value(), "custom render requires --height");
  require(options.precision.has_value(), "custom render requires --precision");
  require(options.policy.has_value(), "custom render requires --policy");

  auto const scene = parse_scene(*options.scene);
  auto const rendering = parse_rendering(*options.rendering);
  auto const width = parse_unsigned<std::uint32_t>(*options.width, "--width");
  auto const height = parse_unsigned<std::uint32_t>(*options.height, "--height");
  auto const precision = parse_precision(*options.precision);
  auto const policy = parse_policy(*options.policy);

  require(width > 0, "--width must be greater than zero");
  require(height > 0, "--height must be greater than zero");

  std::optional<std::uint32_t> geometry_count;
  if (scene == scene_kind::traversal)
  {
    require(options.geometry_count.has_value(), "traversal scene requires --geometry-count");
    geometry_count = parse_unsigned<std::uint32_t>(*options.geometry_count, "--geometry-count");
    require(*geometry_count > 0, "--geometry-count must be greater than zero");
  }
  else
  {
    require(!options.geometry_count, "--geometry-count is only valid for the traversal scene");
  }

  require(
      scene != scene_kind::rng_probe || rendering == rendering_kind::randomized,
      "rng-probe scene requires randomized rendering");

  std::optional<std::uint32_t> samples;
  std::optional<std::uint64_t> seed;
  sensor_kind sensor = sensor_kind::point;

  if (rendering == rendering_kind::randomized)
  {
    require(options.samples.has_value(), "randomized rendering requires --samples");
    samples = parse_unsigned<std::uint32_t>(*options.samples, "--samples");
    require(*samples > 0, "--samples must be greater than zero");
    sensor = sensor_kind::uniform;

    if (options.seed && *options.seed != "none")
    {
      seed = parse_unsigned<std::uint64_t>(*options.seed, "--seed");
    }
  }
  else
  {
    require(!options.samples, "--samples is invalid for deterministic rendering");
    require(!options.seed, "--seed is invalid for deterministic rendering");
  }

  auto const warmup = options.warmup ? parse_unsigned<std::uint32_t>(*options.warmup, "--warmup") : 1U;
  auto const repetitions =
      options.repetitions ? parse_unsigned<std::uint32_t>(*options.repetitions, "--repetitions") : 9U;
  require(repetitions > 0, "--repetitions must be greater than zero");

  require(
      width <= std::numeric_limits<std::uint32_t>::max() / height,
      "requested image dimensions overflow htracer's pixel count");
  auto const pixel_count = static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
  auto const bytes_per_pixel = precision == precision_kind::f32 ? std::size_t{12} : std::size_t{24};
  require(
      pixel_count <= std::numeric_limits<std::size_t>::max() / bytes_per_pixel,
      "requested image dimensions overflow addressable storage");

  return {
      .id = "custom/render",
      .canonical = false,
      .render =
          {.scene = scene,
           .rendering = rendering,
           .precision = precision,
           .policy = policy,
           .batcher = batcher_kind::column,
           .sensor = sensor,
           .lens = lens_kind::pinhole,
           .width = width,
           .height = height,
           .geometry_count = geometry_count,
           .samples_per_pixel = samples,
           .seed = seed},
      .measurement = {.warmup_count = warmup, .repetition_count = repetitions}};
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
    return {.operation = operation_kind::help, .custom_case = std::nullopt, .output_path = std::nullopt};
  }

  if (options.list)
  {
    require(!options.output && !has_custom_options(options), "--list does not accept additional options");
    return {.operation = operation_kind::list, .custom_case = std::nullopt, .output_path = std::nullopt};
  }

  auto const output_path = options.output ? std::optional<std::filesystem::path>{*options.output} : std::nullopt;

  if (options.suite)
  {
    require(*options.suite == "quick", "--suite only supports: quick");
    require(!has_custom_options(options), "--suite quick only accepts --output");
    return {.operation = operation_kind::quick_suite, .custom_case = std::nullopt, .output_path = output_path};
  }

  return {
      .operation = operation_kind::custom_render, .custom_case = make_custom_case(options), .output_path = output_path};
}


void
print_help(std::ostream &output)
{
  output << "Usage:\n"
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
            "  --output PATH       Write versioned JSON in addition to console output\n";
}

} // namespace htracer::benchmarks
