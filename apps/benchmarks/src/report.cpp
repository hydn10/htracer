#include <htracer_benchmarks/report.hpp>

#include <htracer_benchmarks/model.hpp>
#include <htracer_benchmarks/suite.hpp>

#include <cerrno>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <format>
#include <optional>
#include <print>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <utility>
#include <variant>


namespace htracer::benchmarks
{

namespace
{

[[nodiscard]]
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


[[nodiscard]]
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


[[nodiscard]]
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


[[nodiscard]]
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


[[nodiscard]]
std::uint32_t
effective_samples(render_mode const &rendering)
{
  return std::visit(
      []<typename Mode>(Mode const &mode)
  {
    if constexpr (std::same_as<Mode, deterministic_render>)
    {
      return std::uint32_t{1};
    }
    else
    {
      static_assert(std::same_as<Mode, randomized_render>);
      return mode.samples().value;
    }
  },
      rendering);
}


[[nodiscard]]
long double
pixels_per_second(benchmark_result const &result, benchmark_definition const &definition)
{
  auto const pixels = static_cast<long double>(definition.extent().pixel_count());
  auto const seconds = static_cast<long double>(result.summary().median.count()) / 1'000'000'000.0L;
  return pixels / seconds;
}


[[nodiscard]]
std::string
hex_checksum(image_checksum checksum)
{
  return std::format("0x{:016x}", checksum.value());
}


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
std::string_view
seed_id(randomized_render const &rendering) noexcept
{
  return rendering.seed() ? "seeded" : "unseeded";
}


[[nodiscard]]
std::string
canonical_benchmark_name(benchmark_definition const &definition)
{
  auto const precision = precision_id(definition.precision());
  auto const policy = policy_name(definition.policy());

  return std::visit(
      [&]<typename Scene>(Scene const &scene) -> std::string
  {
    return std::visit(
        [&]<typename Mode>(Mode const &mode) -> std::string
    {
      if constexpr (std::same_as<Scene, mixed_scene>)
      {
        if constexpr (std::same_as<Mode, deterministic_render>)
        {
          return std::format("quick.v1/mixed/deterministic/{}/{}", precision, policy);
        }
        else
        {
          static_assert(std::same_as<Mode, randomized_render>);
          return std::format("quick.v1/mixed/randomized-{}/{}/{}", seed_id(mode), precision, policy);
        }
      }
      else if constexpr (std::same_as<Scene, traversal_scene>)
      {
        if constexpr (std::same_as<Mode, deterministic_render>)
        {
          return std::format("quick.v1/traversal/g{}/deterministic/{}/{}", scene.count.value(), precision, policy);
        }
        else
        {
          static_assert(std::same_as<Mode, randomized_render>);
          return std::format(
              "quick.v1/traversal/g{}/randomized-{}/spp{}/{}/{}",
              scene.count.value(),
              seed_id(mode),
              mode.samples().value,
              precision,
              policy);
        }
      }
      else
      {
        static_assert(std::same_as<Scene, rng_probe_scene>);
        if constexpr (std::same_as<Mode, deterministic_render>)
        {
          throw std::logic_error("canonical RNG probe cannot use deterministic rendering");
        }
        else
        {
          static_assert(std::same_as<Mode, randomized_render>);
          return std::format(
              "quick.v1/rng-probe/{}/spp{}/{}/{}", seed_id(mode), mode.samples().value, precision, policy);
        }
      }
    },
        definition.rendering());
  },
      definition.scene());
}


class json_writer
{
public:
  explicit json_writer(std::filesystem::path const &path)
  {
#if defined(_WIN32)
    if (_wfopen_s(&file_, path.c_str(), L"wb") != 0) // NOLINT(misc-include-cleaner)
    {
      file_ = nullptr;
    }
#else
    file_ = std::fopen(path.c_str(), "wb");
#endif
    if (file_ == nullptr)
    {
      throw std::system_error(errno, std::generic_category(), "failed to open JSON output");
    }
  }

  json_writer(json_writer const &) = delete;
  json_writer &
  operator=(json_writer const &) = delete;
  json_writer(json_writer &&) = delete;
  json_writer &
  operator=(json_writer &&) = delete;

  ~json_writer()
  {
    if (file_ != nullptr)
    {
      (void)std::fclose(file_); // NOLINT(cppcoreguidelines-owning-memory)
    }
  }

  template<typename... Args>
  void
  write(std::format_string<Args...> format, Args &&...args)
  {
    std::print(file_, format, std::forward<Args>(args)...);
  }

  void
  string(std::string_view value)
  {
    write("\"");
    for (unsigned char const character : value)
    {
      switch (character)
      {
      case '"':
        write("\\\"");
        break;
      case '\\':
        write("\\\\");
        break;
      case '\b':
        write("\\b");
        break;
      case '\f':
        write("\\f");
        break;
      case '\n':
        write("\\n");
        break;
      case '\r':
        write("\\r");
        break;
      case '\t':
        write("\\t");
        break;
      default:
        if (character < 0x20)
        {
          write("\\u{:04x}", static_cast<unsigned>(character));
        }
        else if (std::fputc(character, file_) == EOF)
        {
          throw std::system_error(errno, std::generic_category(), "failed to write JSON output");
        }
      }
    }
    write("\"");
  }

  void
  close()
  {
    auto *const file = std::exchange(file_, nullptr);
    if (std::fclose(file) != 0) // NOLINT(cppcoreguidelines-owning-memory)
    {
      throw std::system_error(errno, std::generic_category(), "failed to close JSON output");
    }
  }

private:
  std::FILE *file_{};
};


void
write_environment_json(json_writer &output, environment_info const &environment)
{
  output.write("    \"os\": ");
  output.string(environment.os);
  output.write(",\n    \"architecture\": ");
  output.string(environment.architecture);
  output.write(",\n    \"compiler\": ");
  output.string(environment.compiler);
  output.write(",\n    \"compiler_version\": ");
  output.string(environment.compiler_version);
  output.write(",\n    \"build_type\": ");
  output.string(environment.build_type);
  output.write(",\n    \"logical_processors\": {}\n", environment.logical_processors);
}


void
write_scene_json(json_writer &output, scene_spec const &scene)
{
  output.string(scene_name(scene));
  output.write(",\n      \"geometry_count\": ");
  std::visit(
      [&output]<typename Scene>(Scene const &value)
  {
    if constexpr (std::same_as<Scene, traversal_scene>)
    {
      output.write("{}", value.count.value());
    }
    else
    {
      output.write("null");
    }
  },
      scene);
}


void
write_rendering_json(json_writer &output, render_mode const &rendering)
{
  output.string(rendering_name(rendering));
  std::visit(
      [&output]<typename Mode>(Mode const &mode)
  {
    if constexpr (std::same_as<Mode, deterministic_render>)
    {
      output.write(",\n      \"sensor\": \"point\"");
      output.write(",\n      \"samples_per_pixel\": null");
      output.write(",\n      \"seed\": null");
    }
    else
    {
      static_assert(std::same_as<Mode, randomized_render>);
      output.write(",\n      \"sensor\": \"uniform\"");
      output.write(",\n      \"samples_per_pixel\": {}", mode.samples().value);
      output.write(",\n      \"seed\": ");
      if (mode.seed())
      {
        output.string(std::to_string(mode.seed()->value));
      }
      else
      {
        output.write("null");
      }
    }
  },
      rendering);
}


void
write_result_json(json_writer &output, benchmark_result const &result)
{
  auto const &definition = result.benchmark().definition();
  auto const canonical = result.benchmark().is_canonical();
  auto const summary = result.summary();

  output.write("    {{\n      \"id\": ");
  output.string(benchmark_name(result.benchmark()));
  output.write(",\n      \"canonical\": {}", canonical);
  output.write(",\n      \"benchmark\": \"render\"");
  output.write(",\n      \"scene\": ");
  write_scene_json(output, definition.scene());
  output.write(",\n      \"rendering\": ");
  write_rendering_json(output, definition.rendering());
  output.write(",\n      \"precision\": ");
  output.string(precision_name(definition.precision()));
  output.write(",\n      \"policy\": ");
  output.string(policy_name(definition.policy()));
  output.write(",\n      \"batcher\": \"column\"");
  output.write(",\n      \"lens\": \"pinhole\"");
  output.write(",\n      \"width\": {}", definition.extent().width());
  output.write(",\n      \"height\": {}", definition.extent().height());
  output.write(",\n      \"warmup_count\": {}", definition.measurement().warmups.value());
  output.write(",\n      \"repetition_count\": {}", definition.measurement().repetitions.value());
  output.write(",\n      \"samples_ns\": [");
  for (std::size_t index = 0; index < result.renders().size(); ++index)
  {
    if (index != 0)
    {
      output.write(", ");
    }
    output.write("{}", result.renders()[index].duration.count());
  }
  output.write("]");
  output.write(",\n      \"minimum_ns\": {}", summary.minimum.count());
  output.write(",\n      \"median_ns\": {}", summary.median.count());
  output.write(",\n      \"maximum_ns\": {}", summary.maximum.count());
  auto const pixels = pixels_per_second(result, definition);
  output.write(",\n      \"pixels_per_second\": {}", pixels);
  output.write(",\n      \"primary_samples_per_second\": {}", pixels * effective_samples(definition.rendering()));
  output.write(",\n      \"checksum\": ");
  output.string(hex_checksum(result.checksum()));
  output.write(",\n      \"warmup_checksum\": ");
  if (result.warmup_checksum())
  {
    output.string(hex_checksum(*result.warmup_checksum()));
  }
  else
  {
    output.write("null");
  }
  output.write("\n    }}");
}

} // namespace


std::string
benchmark_name(benchmark_case const &benchmark)
{
  if (!benchmark.is_canonical())
  {
    return "custom/render";
  }
  return canonical_benchmark_name(benchmark.definition());
}


environment_info
get_environment_info()
{
#if defined(_WIN32)
  std::string operating_system = "windows";
#elif defined(__linux__)
  std::string operating_system = "linux";
#else
  std::string operating_system = "unknown";
#endif

#if defined(_M_X64) || defined(__x86_64__)
  std::string architecture = "x86_64";
#elif defined(_M_ARM64) || defined(__aarch64__)
  std::string architecture = "arm64";
#else
  std::string architecture = "unknown";
#endif

#if defined(__clang__)
  std::string compiler = "clang";
  std::string compiler_version = std::to_string(__clang_major__) + "." + std::to_string(__clang_minor__) + "." +
                                 std::to_string(__clang_patchlevel__);
#elif defined(_MSC_VER)
  std::string compiler = "msvc";
  std::string compiler_version = std::to_string(_MSC_FULL_VER);
#elif defined(__GNUC__)
  std::string compiler = "gcc";
  std::string compiler_version =
      std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__) + "." + std::to_string(__GNUC_PATCHLEVEL__);
#else
  std::string compiler = "unknown";
  std::string compiler_version = "unknown";
#endif

  return {
      .os = std::move(operating_system),
      .architecture = std::move(architecture),
      .compiler = std::move(compiler),
      .compiler_version = std::move(compiler_version),
      .build_type = HTRACER_BENCHMARK_BUILD_TYPE,
      .logical_processors = std::thread::hardware_concurrency()};
}


void
print_case_list(std::span<benchmark_case const> benchmarks)
{
  std::println("quick suite ({} cases):", benchmarks.size());
  for (auto const &benchmark : benchmarks)
  {
    auto const &definition = benchmark.definition();
    std::print(
        "  {} [{}x{}, {}, {}, {}, {}",
        benchmark_name(benchmark),
        definition.extent().width(),
        definition.extent().height(),
        scene_name(definition.scene()),
        rendering_name(definition.rendering()),
        precision_name(definition.precision()),
        policy_name(definition.policy()));
    std::visit(
        []<typename Mode>(Mode const &mode)
    {
      if constexpr (std::same_as<Mode, randomized_render>)
      {
        std::print(", spp={}", mode.samples().value);
      }
    },
        definition.rendering());
    std::println("]");
  }
}


void
print_environment(environment_info const &environment)
{
  std::println(
      "htracer benchmarks\n"
      "  OS: {} {}\n"
      "  Compiler: {} {}\n"
      "  Build: {}\n"
      "  Logical processors: {}\n",
      environment.os,
      environment.architecture,
      environment.compiler,
      environment.compiler_version,
      environment.build_type,
      environment.logical_processors);
}


void
print_result(benchmark_result const &result)
{
  auto const &definition = result.benchmark().definition();
  auto const summary = result.summary();
  auto const ns_to_ms = [](std::chrono::nanoseconds duration)
  {
    return static_cast<long double>(duration.count()) / 1'000'000.0L;
  };
  auto const pixels = pixels_per_second(result, definition);
  std::println(
      "{}\n"
      "  median/min/max: {:.3f} / {:.3f} / {:.3f} ms\n"
      "  throughput: {:.3f} Mpixel/s, {:.3f} Mprimary-sample/s\n"
      "  checksum: {}\n",
      benchmark_name(result.benchmark()),
      ns_to_ms(summary.median),
      ns_to_ms(summary.minimum),
      ns_to_ms(summary.maximum),
      pixels / 1'000'000.0L,
      pixels * effective_samples(definition.rendering()) / 1'000'000.0L,
      hex_checksum(result.checksum()));
}


void
write_json(std::filesystem::path const &path, run_report const &report)
{
  json_writer output{path};
  output.write(
      "{{\n  \"schema_version\": {},\n  \"workload_version\": {},\n  \"environment\": {{\n",
      output_schema_version,
      workload_schema_version);
  write_environment_json(output, report.environment);
  output.write("  }},\n  \"results\": [\n");
  for (std::size_t index = 0; index < report.results.size(); ++index)
  {
    if (index != 0)
    {
      output.write(",\n");
    }
    write_result_json(output, report.results[index]);
  }
  output.write("\n  ]\n}}\n");
  output.close();
}

} // namespace htracer::benchmarks
