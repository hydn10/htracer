#include <htracer_benchmarks/report.hpp>

#include <htracer_benchmarks/model.hpp>
#include <htracer_benchmarks/suite.hpp>

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
#include <limits>
#include <locale>
#include <optional>
#include <ostream>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <utility>


namespace htracer::benchmarks
{

namespace
{

template<typename Enum>
[[nodiscard]]
std::string_view
unknown_enum([[maybe_unused]] Enum value) noexcept
{
  return "unknown";
}


[[nodiscard]]
std::uint32_t
effective_samples(render_configuration const &configuration) noexcept
{
  return configuration.samples_per_pixel.value_or(1);
}


[[nodiscard]]
long double
pixels_per_second(benchmark_result const &result)
{
  auto const pixels = static_cast<long double>(result.benchmark.render.width) * result.benchmark.render.height;
  auto const seconds = static_cast<long double>(result.summary.median_ns) / 1'000'000'000.0L;
  return pixels / seconds;
}


[[nodiscard]]
long double
primary_samples_per_second(benchmark_result const &result)
{
  return pixels_per_second(result) * effective_samples(result.benchmark.render);
}


[[nodiscard]]
std::string
hex_checksum(std::uint64_t checksum)
{
  std::ostringstream output;
  output << "0x" << std::hex << std::setw(16) << std::setfill('0') << checksum;
  return output.str();
}


void
write_json_string(std::ostream &output, std::string_view value)
{
  output << '"';
  for (unsigned char const character : value)
  {
    switch (character)
    {
    case '"':
      output << "\\\"";
      break;
    case '\\':
      output << "\\\\";
      break;
    case '\b':
      output << "\\b";
      break;
    case '\f':
      output << "\\f";
      break;
    case '\n':
      output << "\\n";
      break;
    case '\r':
      output << "\\r";
      break;
    case '\t':
      output << "\\t";
      break;
    default:
      if (character < 0x20)
      {
        output << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<unsigned>(character) << std::dec
               << std::setfill(' ');
      }
      else
      {
        output << static_cast<char>(character);
      }
    }
  }
  output << '"';
}


template<typename T>
void
write_optional_integer(std::ostream &output, std::optional<T> value)
{
  if (value)
  {
    output << *value;
  }
  else
  {
    output << "null";
  }
}


void
write_optional_seed(std::ostream &output, std::optional<std::uint64_t> seed)
{
  if (seed)
  {
    write_json_string(output, std::to_string(*seed));
  }
  else
  {
    output << "null";
  }
}


void
write_environment_json(std::ostream &output, environment_info const &environment)
{
  output << "    \"os\": ";
  write_json_string(output, environment.os);
  output << ",\n    \"architecture\": ";
  write_json_string(output, environment.architecture);
  output << ",\n    \"compiler\": ";
  write_json_string(output, environment.compiler);
  output << ",\n    \"compiler_version\": ";
  write_json_string(output, environment.compiler_version);
  output << ",\n    \"build_type\": ";
  write_json_string(output, environment.build_type);
  output << ",\n    \"logical_processors\": " << environment.logical_processors << '\n';
}


void
write_result_json(std::ostream &output, benchmark_result const &result)
{
  auto const &benchmark = result.benchmark;
  auto const &render = benchmark.render;

  output << "    {\n      \"id\": ";
  write_json_string(output, benchmark.id);
  output << ",\n      \"canonical\": " << (benchmark.canonical ? "true" : "false")
         << ",\n      \"benchmark\": \"render\""
         << ",\n      \"scene\": ";
  write_json_string(output, to_string(render.scene));
  output << ",\n      \"rendering\": ";
  write_json_string(output, to_string(render.rendering));
  output << ",\n      \"precision\": ";
  write_json_string(output, to_string(render.precision));
  output << ",\n      \"policy\": ";
  write_json_string(output, to_string(render.policy));
  output << ",\n      \"batcher\": ";
  write_json_string(output, to_string(render.batcher));
  output << ",\n      \"sensor\": ";
  write_json_string(output, to_string(render.sensor));
  output << ",\n      \"lens\": ";
  write_json_string(output, to_string(render.lens));
  output << ",\n      \"width\": " << render.width << ",\n      \"height\": " << render.height
         << ",\n      \"geometry_count\": ";
  write_optional_integer(output, render.geometry_count);
  output << ",\n      \"samples_per_pixel\": ";
  write_optional_integer(output, render.samples_per_pixel);
  output << ",\n      \"seed\": ";
  write_optional_seed(output, render.seed);
  output << ",\n      \"warmup_count\": " << benchmark.measurement.warmup_count
         << ",\n      \"repetition_count\": " << benchmark.measurement.repetition_count << ",\n      \"samples_ns\": [";

  for (std::size_t index = 0; index < result.samples_ns.size(); ++index)
  {
    if (index != 0)
    {
      output << ", ";
    }
    output << result.samples_ns[index];
  }

  output << "]"
         << ",\n      \"minimum_ns\": " << result.summary.minimum_ns
         << ",\n      \"median_ns\": " << result.summary.median_ns
         << ",\n      \"maximum_ns\": " << result.summary.maximum_ns
         << ",\n      \"pixels_per_second\": " << pixels_per_second(result)
         << ",\n      \"primary_samples_per_second\": " << primary_samples_per_second(result)
         << ",\n      \"checksum\": ";
  write_json_string(output, hex_checksum(result.checksum));
  output << "\n    }";
}

} // namespace


std::string_view
to_string(scene_kind value) noexcept
{
  switch (value)
  {
  case scene_kind::mixed:
    return "mixed";
  case scene_kind::traversal:
    return "traversal";
  case scene_kind::rng_probe:
    return "rng-probe";
  }
  return unknown_enum(value);
}


std::string_view
to_string(rendering_kind value) noexcept
{
  switch (value)
  {
  case rendering_kind::deterministic:
    return "deterministic";
  case rendering_kind::randomized:
    return "randomized";
  }
  return unknown_enum(value);
}


std::string_view
to_string(precision_kind value) noexcept
{
  switch (value)
  {
  case precision_kind::f32:
    return "float";
  case precision_kind::f64:
    return "double";
  }
  return unknown_enum(value);
}


std::string_view
to_string(policy_kind value) noexcept
{
  switch (value)
  {
  case policy_kind::seq:
    return "seq";
  case policy_kind::par:
    return "par";
  }
  return unknown_enum(value);
}


std::string_view
to_string(batcher_kind value) noexcept
{
  switch (value)
  {
  case batcher_kind::column:
    return "column";
  }
  return unknown_enum(value);
}


std::string_view
to_string(sensor_kind value) noexcept
{
  switch (value)
  {
  case sensor_kind::point:
    return "point";
  case sensor_kind::uniform:
    return "uniform";
  }
  return unknown_enum(value);
}


std::string_view
to_string(lens_kind value) noexcept
{
  switch (value)
  {
  case lens_kind::pinhole:
    return "pinhole";
  }
  return unknown_enum(value);
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
print_case_list(std::ostream &output, std::span<benchmark_case const> benchmarks)
{
  output << "quick suite (" << benchmarks.size() << " cases):\n";
  for (auto const &benchmark : benchmarks)
  {
    auto const &render = benchmark.render;
    output << "  " << benchmark.id << " [" << render.width << 'x' << render.height << ", " << to_string(render.scene)
           << ", " << to_string(render.rendering) << ", " << to_string(render.precision) << ", "
           << to_string(render.policy);
    if (render.samples_per_pixel)
    {
      output << ", spp=" << *render.samples_per_pixel;
    }
    output << "]\n";
  }
}


void
print_environment(std::ostream &output, environment_info const &environment)
{
  output << "htracer benchmarks\n"
         << "  OS: " << environment.os << ' ' << environment.architecture << '\n'
         << "  Compiler: " << environment.compiler << ' ' << environment.compiler_version << '\n'
         << "  Build: " << environment.build_type << '\n'
         << "  Logical processors: " << environment.logical_processors << "\n\n";
}


void
print_result(std::ostream &output, benchmark_result const &result)
{
  auto const ns_to_ms = [](std::uint64_t nanoseconds)
  {
    return static_cast<long double>(nanoseconds) / 1'000'000.0L;
  };

  output << result.benchmark.id << '\n'
         << std::fixed << std::setprecision(3) << "  median/min/max: " << ns_to_ms(result.summary.median_ns) << " / "
         << ns_to_ms(result.summary.minimum_ns) << " / " << ns_to_ms(result.summary.maximum_ns) << " ms\n"
         << "  throughput: " << pixels_per_second(result) / 1'000'000.0L << " Mpixel/s, "
         << primary_samples_per_second(result) / 1'000'000.0L << " Mprimary-sample/s\n"
         << "  checksum: " << hex_checksum(result.checksum) << "\n\n";
}


void
write_json(std::filesystem::path const &path, run_report const &report)
{
  std::ofstream output{path};
  if (!output)
  {
    throw std::runtime_error("failed to open JSON output: " + path.string());
  }

  output.imbue(std::locale::classic());
  output << std::setprecision(std::numeric_limits<long double>::max_digits10)
         << "{\n  \"schema_version\": " << output_schema_version
         << ",\n  \"workload_version\": " << workload_schema_version << ",\n  \"environment\": {\n";
  write_environment_json(output, report.environment);
  output << "  },\n  \"results\": [\n";

  for (std::size_t index = 0; index < report.results.size(); ++index)
  {
    if (index != 0)
    {
      output << ",\n";
    }
    write_result_json(output, report.results[index]);
  }

  output << "\n  ]\n}\n";
  output.close();
  if (!output)
  {
    throw std::runtime_error("failed to write JSON output: " + path.string());
  }
}

} // namespace htracer::benchmarks
