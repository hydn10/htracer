#include <htracer_benchmarks/reporting/json.hpp>

#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/benchmark_result.hpp>
#include <htracer_benchmarks/quick_suite.hpp>
#include <htracer_benchmarks/render_mode.hpp>
#include <htracer_benchmarks/reporting/benchmark_name.hpp>
#include <htracer_benchmarks/reporting/json_writer.hpp>
#include <htracer_benchmarks/reporting/result_values.hpp>
#include <htracer_benchmarks/run_report.hpp>
#include <htracer_benchmarks/scene_spec.hpp>

#include <concepts>
#include <filesystem>
#include <format>
#include <string>
#include <variant>


namespace htracer::benchmarks::reporting
{

namespace
{

void
write_environment(json_writer &output, environment_info const &environment)
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
write_scene(json_writer &output, scene_spec const &scene)
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
write_rendering(json_writer &output, render_mode const &rendering)
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
      output.write(std::format(",\n      \"samples_per_pixel\": {}", mode.samples().value));
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
write_result(json_writer &output, benchmark_result const &result)
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
  output.write(",\n      \"batcher\": \"column\"");
  output.write(",\n      \"lens\": \"pinhole\"");
  output.write(std::format(",\n      \"width\": {}", definition.extent().width()));
  output.write(std::format(",\n      \"height\": {}", definition.extent().height()));
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
    output.write(std::format("{}", render.duration.count()));
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
  if (result.warmup_checksum())
  {
    output.string(hex_checksum(*result.warmup_checksum()));
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
  json_writer output{path};
  output.write(std::format(
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
  output.close();
}

} // namespace htracer::benchmarks::reporting
