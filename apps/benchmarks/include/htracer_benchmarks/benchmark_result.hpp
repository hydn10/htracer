#ifndef HTRACER_BENCHMARKS_BENCHMARK_RESULT_HPP
#define HTRACER_BENCHMARKS_BENCHMARK_RESULT_HPP


#include <htracer_benchmarks/benchmark_case.hpp>

#include <chrono>
#include <cstdint>
#include <optional>
#include <vector>


namespace htracer::benchmarks
{

class image_checksum
{
  std::uint64_t value_;

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
};


class run_checksum
{
  std::uint64_t value_;

public:
  explicit constexpr run_checksum(std::uint64_t value) noexcept
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
  operator==(run_checksum const &, run_checksum const &) noexcept = default;
};


class measured_render
{
  std::chrono::nanoseconds duration_;
  image_checksum checksum_;

  constexpr measured_render(std::chrono::nanoseconds duration, image_checksum checksum) noexcept
      : duration_{duration}
      , checksum_{checksum}
  {
  }

public:
  [[nodiscard]]
  static measured_render
  make(std::chrono::nanoseconds duration, image_checksum checksum);

  [[nodiscard]]
  constexpr std::chrono::nanoseconds
  duration() const noexcept
  {
    return duration_;
  }

  [[nodiscard]]
  constexpr image_checksum
  checksum() const noexcept
  {
    return checksum_;
  }
};


struct duration_summary
{
  std::chrono::nanoseconds minimum;
  std::chrono::nanoseconds median;
  std::chrono::nanoseconds maximum;
};


class benchmark_result
{
  benchmark_case benchmark_;
  std::vector<measured_render> renders_;
  duration_summary summary_;
  run_checksum checksum_;
  std::vector<image_checksum> warmup_checksums_;
  std::optional<run_checksum> warmup_checksum_;

  benchmark_result(
      benchmark_case benchmark,
      std::vector<measured_render> renders,
      duration_summary summary,
      run_checksum checksum,
      std::vector<image_checksum> warmup_checksums,
      std::optional<run_checksum> warmup_checksum);

public:
  [[nodiscard]]
  static benchmark_result
  make(benchmark_case benchmark, std::vector<measured_render> renders, std::vector<image_checksum> warmup_checksums);

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
  constexpr run_checksum
  checksum() const noexcept
  {
    return checksum_;
  }

  [[nodiscard]]
  constexpr std::optional<run_checksum>
  warmup_checksum() const noexcept
  {
    return warmup_checksum_;
  }

  [[nodiscard]]
  constexpr std::vector<image_checksum> const &
  warmup_checksums() const noexcept
  {
    return warmup_checksums_;
  }
};

} // namespace htracer::benchmarks

#endif
