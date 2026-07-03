#ifndef HTRACER_BENCHMARKS_MODEL_HPP
#define HTRACER_BENCHMARKS_MODEL_HPP


#include <htracer/rendering/random_seed.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <variant>
#include <vector>


namespace htracer::benchmarks
{

class image_extent
{
public:
  [[nodiscard]]
  static image_extent
  make(std::uint32_t width, std::uint32_t height);

  [[nodiscard]]
  constexpr std::uint32_t
  width() const noexcept
  {
    return width_;
  }

  [[nodiscard]]
  constexpr std::uint32_t
  height() const noexcept
  {
    return height_;
  }

  [[nodiscard]]
  constexpr std::size_t
  pixel_count() const noexcept
  {
    return static_cast<std::size_t>(width_) * height_;
  }

private:
  constexpr image_extent(std::uint32_t width, std::uint32_t height) noexcept
      : width_{width}
      , height_{height}
  {
  }

  std::uint32_t width_;
  std::uint32_t height_;
};


class geometry_count
{
public:
  [[nodiscard]]
  static geometry_count
  make(std::uint32_t value);

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept
  {
    return value_;
  }

private:
  explicit constexpr geometry_count(std::uint32_t value) noexcept
      : value_{value}
  {
  }

  std::uint32_t value_;
};


class repetition_count
{
public:
  [[nodiscard]]
  static repetition_count
  make(std::uint32_t value);

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept
  {
    return value_;
  }

private:
  explicit constexpr repetition_count(std::uint32_t value) noexcept
      : value_{value}
  {
  }

  std::uint32_t value_;
};


class warmup_count
{
public:
  explicit constexpr warmup_count(std::uint32_t value) noexcept
      : value_{value}
  {
  }

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept
  {
    return value_;
  }

private:
  std::uint32_t value_;
};


struct mixed_scene
{
};

struct traversal_scene
{
  geometry_count count;
};

struct rng_probe_scene
{
};

using scene_spec = std::variant<mixed_scene, traversal_scene, rng_probe_scene>;


struct deterministic_render
{
};

class randomized_render
{
public:
  [[nodiscard]]
  static randomized_render
  make(htracer::rendering::samples_per_pixel samples, std::optional<htracer::rendering::random_seed> seed);

  [[nodiscard]]
  constexpr htracer::rendering::samples_per_pixel
  samples() const noexcept
  {
    return samples_;
  }

  [[nodiscard]]
  constexpr std::optional<htracer::rendering::random_seed>
  seed() const noexcept
  {
    return seed_;
  }

private:
  constexpr randomized_render(
      htracer::rendering::samples_per_pixel samples, std::optional<htracer::rendering::random_seed> seed) noexcept
      : samples_{samples}
      , seed_{seed}
  {
  }

  htracer::rendering::samples_per_pixel samples_;
  std::optional<htracer::rendering::random_seed> seed_;
};

using render_mode = std::variant<deterministic_render, randomized_render>;


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


struct measurement_plan
{
  warmup_count warmups;
  repetition_count repetitions;
};


class benchmark_definition
{
public:
  [[nodiscard]]
  static benchmark_definition
  deterministic(
      scene_spec scene,
      precision_kind precision,
      policy_kind policy,
      image_extent extent,
      measurement_plan measurement);

  [[nodiscard]]
  static benchmark_definition
  randomized(
      scene_spec scene,
      randomized_render rendering,
      precision_kind precision,
      policy_kind policy,
      image_extent extent,
      measurement_plan measurement);

  [[nodiscard]]
  constexpr scene_spec const &
  scene() const noexcept
  {
    return scene_;
  }
  [[nodiscard]]
  constexpr render_mode const &
  rendering() const noexcept
  {
    return rendering_;
  }
  [[nodiscard]]
  constexpr precision_kind
  precision() const noexcept
  {
    return precision_;
  }
  [[nodiscard]]
  constexpr policy_kind
  policy() const noexcept
  {
    return policy_;
  }
  [[nodiscard]]
  constexpr image_extent
  extent() const noexcept
  {
    return extent_;
  }
  [[nodiscard]]
  constexpr measurement_plan
  measurement() const noexcept
  {
    return measurement_;
  }

private:
  benchmark_definition(
      scene_spec scene,
      render_mode rendering,
      precision_kind precision,
      policy_kind policy,
      image_extent extent,
      measurement_plan measurement);

  scene_spec scene_;
  render_mode rendering_;
  precision_kind precision_;
  policy_kind policy_;
  image_extent extent_;
  measurement_plan measurement_;
};


class quick_suite_catalog;


class benchmark_case
{
public:
  [[nodiscard]]
  static benchmark_case
  custom(benchmark_definition definition);

  [[nodiscard]]
  constexpr benchmark_definition const &
  definition() const noexcept
  {
    return definition_;
  }

  [[nodiscard]]
  constexpr bool
  is_canonical() const noexcept
  {
    return origin_ == origin::canonical;
  }

private:
  friend class quick_suite_catalog;

  enum class origin
  {
    canonical,
    custom
  };

  [[nodiscard]]
  static benchmark_case
  canonical(benchmark_definition definition);

  benchmark_case(origin provenance, benchmark_definition definition);

  origin origin_;
  benchmark_definition definition_;
};


class image_checksum
{
public:
  explicit constexpr image_checksum(std::uint64_t value) noexcept
      : value_{value}
  {
  }

  [[nodiscard]]
  constexpr std::uint64_t
  value() const noexcept
  {
    return value_;
  }

  friend constexpr bool
  operator==(image_checksum const &, image_checksum const &) noexcept = default;

private:
  std::uint64_t value_;
};


struct measured_render
{
  std::chrono::nanoseconds duration;
  image_checksum checksum;
};


struct duration_summary
{
  std::chrono::nanoseconds minimum;
  std::chrono::nanoseconds median;
  std::chrono::nanoseconds maximum;
};


class benchmark_result
{
public:
  [[nodiscard]]
  static benchmark_result
  make(benchmark_case benchmark, std::vector<measured_render> renders, std::optional<image_checksum> warmup_checksum);

  [[nodiscard]]
  constexpr benchmark_case const &
  benchmark() const noexcept
  {
    return benchmark_;
  }
  [[nodiscard]]
  constexpr std::vector<measured_render> const &
  renders() const noexcept
  {
    return renders_;
  }
  [[nodiscard]]
  constexpr duration_summary
  summary() const noexcept
  {
    return summary_;
  }
  [[nodiscard]]
  constexpr image_checksum
  checksum() const noexcept
  {
    return checksum_;
  }
  [[nodiscard]]
  constexpr std::optional<image_checksum>
  warmup_checksum() const noexcept
  {
    return warmup_checksum_;
  }

private:
  benchmark_result(
      benchmark_case benchmark,
      std::vector<measured_render> renders,
      duration_summary summary,
      image_checksum checksum,
      std::optional<image_checksum> warmup_checksum);

  benchmark_case benchmark_;
  std::vector<measured_render> renders_;
  duration_summary summary_;
  image_checksum checksum_;
  std::optional<image_checksum> warmup_checksum_;
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


struct help_command
{
};

struct list_command
{
};

struct quick_suite_command
{
  std::optional<std::filesystem::path> output;
};

struct custom_render_command
{
  benchmark_definition benchmark;
  std::optional<std::filesystem::path> output;
};

using invocation = std::variant<help_command, list_command, quick_suite_command, custom_render_command>;

} // namespace htracer::benchmarks

#endif
