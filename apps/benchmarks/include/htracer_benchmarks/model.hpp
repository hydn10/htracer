#ifndef HTRACER_BENCHMARKS_MODEL_HPP
#define HTRACER_BENCHMARKS_MODEL_HPP


#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>


namespace htracer::benchmarks
{

enum class scene_kind
{
  mixed,
  traversal,
  rng_probe
};


enum class rendering_kind
{
  deterministic,
  randomized
};


enum class precision_kind
{
  f32,
  f64
};


enum class policy_kind
{
  seq,
  par
};


enum class batcher_kind
{
  column
};


enum class sensor_kind
{
  point,
  uniform
};


enum class lens_kind
{
  pinhole
};


struct render_configuration
{
  scene_kind scene;
  rendering_kind rendering;
  precision_kind precision;
  policy_kind policy;
  batcher_kind batcher;
  sensor_kind sensor;
  lens_kind lens;
  std::uint32_t width;
  std::uint32_t height;
  std::optional<std::uint32_t> geometry_count;
  std::optional<std::uint32_t> samples_per_pixel;
  std::optional<std::uint64_t> seed;
};


struct measurement_configuration
{
  std::uint32_t warmup_count;
  std::uint32_t repetition_count;
};


struct benchmark_case
{
  std::string id;
  bool canonical;
  render_configuration render;
  measurement_configuration measurement;
};


struct duration_summary
{
  std::uint64_t minimum_ns;
  std::uint64_t median_ns;
  std::uint64_t maximum_ns;
};


struct benchmark_result
{
  benchmark_case benchmark;
  std::vector<std::uint64_t> samples_ns;
  duration_summary summary;
  std::uint64_t checksum;
};


struct environment_info
{
  std::string os;
  std::string architecture;
  std::string compiler;
  std::string compiler_version;
  std::string build_type;
  unsigned logical_processors;
};


struct run_report
{
  environment_info environment;
  std::vector<benchmark_result> results;
};


enum class operation_kind
{
  help,
  list,
  quick_suite,
  custom_render
};


struct invocation
{
  operation_kind operation;
  std::optional<benchmark_case> custom_case;
  std::optional<std::filesystem::path> output_path;
};

} // namespace htracer::benchmarks

#endif
