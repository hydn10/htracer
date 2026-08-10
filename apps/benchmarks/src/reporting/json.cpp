#include <htracer_benchmarks/reporting/json.hpp>

#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/benchmark_result.hpp>
#include <htracer_benchmarks/execution/render_pipeline.hpp>
#include <htracer_benchmarks/render_mode.hpp>
#include <htracer_benchmarks/reporting/benchmark_name.hpp>
#include <htracer_benchmarks/reporting/result_values.hpp>
#include <htracer_benchmarks/run_report.hpp>
#include <htracer_benchmarks/scene_spec.hpp>
#include <htracer_benchmarks/schema_version.hpp>

#include <concepts>
#include <filesystem>
#include <format>
#include <fstream>
#include <ios>
#include <string>
#include <string_view>
#include <utility>
#include <variant>


namespace htracer::benchmarks::reporting
{

namespace
{

class json_buffer
{
  std::string value_;

public:
  void
  write(std::string_view value)
  {
    value_.append(value);
  }

  void
  string(std::string_view value)
  {
    write("\"");
    for (auto const raw_character : value)
    {
      auto const character = static_cast<unsigned char>(raw_character);
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
          write(std::format("\\u{:04x}", static_cast<unsigned>(character)));
        }
        else
        {
          value_.push_back(static_cast<char>(character));
        }
      }
    }
    write("\"");
  }

  [[nodiscard]]
  std::string
  finish() && noexcept
  {
    return std::move(value_);
  }
};


void
write_file(std::filesystem::path const &path, std::string_view contents)
{
  std::ofstream file{path, std::ios::binary};
  if (!file)
  {
    throw std::ios_base::failure("failed to open JSON output");
  }

  file.write(contents.data(), static_cast<std::streamsize>(contents.size()));
  if (!file)
  {
    throw std::ios_base::failure("failed to write JSON output");
  }

  file.close();
  if (!file)
  {
    throw std::ios_base::failure("failed to close JSON output");
  }
}


void
write_environment(json_buffer &output, environment_info const &environment)
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
  output.write(std::format(",\n    \"logical_processors\": {}\n", environment.logical_processors));
}


void
write_scene(json_buffer &output, scene_spec const &scene)
{
  output.string(scene_name(scene));
  output.write(",\n      \"geometry_count\": ");
  std::visit(
      [&output]<typename Scene>(Scene const &value)
      {
        if constexpr (std::same_as<Scene, traversal_scene>)
        {
          output.write(std::format("{}", value.count.value()));
        }
        else
        {
          output.write("null");
        }
      },
      scene);
}


void
write_rendering(json_buffer &output, render_mode const &rendering)
{
  output.string(rendering_name(rendering));
  std::visit(
      [&output]<typename Mode>(Mode const &mode)
      {
        using pipeline = execution::render_pipeline<Mode>;

        output.write(",\n      \"sensor\": ");
        output.string(pipeline::sensor_name);
        output.write(",\n      \"batcher\": ");
        output.string(pipeline::batcher_name);
        output.write(",\n      \"lens\": ");
        output.string(pipeline::lens_name);

        if constexpr (std::same_as<Mode, deterministic_render>)
        {
          output.write(",\n      \"samples_per_pixel\": null");
          output.write(",\n      \"seed\": null");
        }
        else
        {
          static_assert(std::same_as<Mode, randomized_render>);
          output.write(std::format(",\n      \"samples_per_pixel\": {}", mode.samples().value()));
          output.write(",\n      \"seed\": ");
          auto const seed = mode.seed();
          if (seed)
          {
            output.string(std::to_string(seed->value));
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
write_result(json_buffer &output, benchmark_result const &result)
{
  auto const &definition = result.benchmark().definition();
  auto const summary = result.summary();

  output.write("    {\n      \"id\": ");
  output.string(benchmark_name(result.benchmark()));
  output.write(std::format(",\n      \"canonical\": {}", result.benchmark().is_canonical()));
  output.write(",\n      \"benchmark\": \"render\"");
  output.write(",\n      \"scene\": ");
  write_scene(output, definition.scene());
  output.write(",\n      \"rendering\": ");
  write_rendering(output, definition.rendering());
  output.write(",\n      \"precision\": ");
  output.string(precision_name(definition.precision()));
  output.write(",\n      \"policy\": ");
  output.string(policy_name(definition.policy()));
  output.write(std::format(",\n      \"width\": {}", definition.extent().width().value()));
  output.write(std::format(",\n      \"height\": {}", definition.extent().height().value()));
  output.write(std::format(",\n      \"warmup_count\": {}", definition.measurement().warmups.value()));
  output.write(std::format(",\n      \"repetition_count\": {}", definition.measurement().repetitions.value()));
  output.write(",\n      \"samples_ns\": [");
  auto first_sample = true;
  for (auto const &render : result.renders())
  {
    if (!first_sample)
    {
      output.write(", ");
    }
    first_sample = false;
    output.write(std::format("{}", render.duration().count()));
  }
  output.write("]");
  output.write(std::format(",\n      \"minimum_ns\": {}", summary.minimum.count()));
  output.write(std::format(",\n      \"median_ns\": {}", summary.median.count()));
  output.write(std::format(",\n      \"maximum_ns\": {}", summary.maximum.count()));
  output.write(std::format(",\n      \"pixels_per_second\": {}", pixels_per_second(result)));
  output.write(std::format(",\n      \"primary_samples_per_second\": {}", primary_samples_per_second(result)));
  output.write(",\n      \"checksum\": ");
  output.string(hex_checksum(result.checksum()));
  output.write(",\n      \"warmup_checksum\": ");
  auto const warmup_checksum = result.warmup_checksum();
  if (warmup_checksum)
  {
    output.string(hex_checksum(*warmup_checksum));
  }
  else
  {
    output.write("null");
  }
  output.write("\n    }");
}

} // namespace


void
write_json(std::filesystem::path const &path, run_report const &report)
{
  json_buffer output;
  output.write(
      std::format(
          "{{\n  \"schema_version\": {},\n  \"workload_version\": {},\n  \"environment\": {{\n",
          output_schema_version,
          workload_schema_version));
  write_environment(output, report.environment);
  output.write("  },\n  \"results\": [\n");
  auto first_result = true;
  for (auto const &result : report.results)
  {
    if (!first_result)
    {
      output.write(",\n");
    }
    first_result = false;
    write_result(output, result);
  }
  output.write("\n  ]\n}\n");
  write_file(path, std::move(output).finish());
}

} // namespace htracer::benchmarks::reporting
