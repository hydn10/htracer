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

} // namespace htracer::benchmarks

#endif
