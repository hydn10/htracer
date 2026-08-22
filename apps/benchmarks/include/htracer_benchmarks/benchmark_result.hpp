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
  explicit constexpr image_checksum(std::uint64_t value) noexcept;

  [[nodiscard]]
  constexpr std::uint64_t
  value() const noexcept;

  friend constexpr bool
  operator==(image_checksum const &left, image_checksum const &right) noexcept;
};


class run_checksum
{
  std::uint64_t value_;

public:
  explicit constexpr run_checksum(std::uint64_t value) noexcept;

  [[nodiscard]]
  constexpr std::uint64_t
  value() const noexcept;

  friend constexpr bool
  operator==(run_checksum const &left, run_checksum const &right) noexcept;
};


class measured_render
{
  std::chrono::nanoseconds duration_;
  image_checksum checksum_;

  constexpr measured_render(std::chrono::nanoseconds duration, image_checksum checksum) noexcept;

public:
  [[nodiscard]]
  static measured_render
  make(std::chrono::nanoseconds duration, image_checksum checksum);

  [[nodiscard]]
  constexpr std::chrono::nanoseconds
  duration() const noexcept;

  [[nodiscard]]
  constexpr image_checksum
  checksum() const noexcept;
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
  benchmark() const noexcept;

  [[nodiscard]]
  constexpr std::vector<measured_render> const &
  renders() const noexcept;

  [[nodiscard]]
  constexpr duration_summary
  summary() const noexcept;

  [[nodiscard]]
  constexpr run_checksum
  checksum() const noexcept;

  [[nodiscard]]
  constexpr std::optional<run_checksum>
  warmup_checksum() const noexcept;

  [[nodiscard]]
  constexpr std::vector<image_checksum> const &
  warmup_checksums() const noexcept;
};


constexpr image_checksum::image_checksum(std::uint64_t value) noexcept
    : value_{value}
{
}


constexpr std::uint64_t
image_checksum::value() const noexcept
{
  return value_;
}


constexpr bool
operator==(image_checksum const &left, image_checksum const &right) noexcept
{
  return left.value_ == right.value_;
}


constexpr run_checksum::run_checksum(std::uint64_t value) noexcept
    : value_{value}
{
}


constexpr std::uint64_t
run_checksum::value() const noexcept
{
  return value_;
}


constexpr bool
operator==(run_checksum const &left, run_checksum const &right) noexcept
{
  return left.value_ == right.value_;
}


constexpr measured_render::measured_render(std::chrono::nanoseconds duration, image_checksum checksum) noexcept
    : duration_{duration}
    , checksum_{checksum}
{
}


constexpr std::chrono::nanoseconds
measured_render::duration() const noexcept
{
  return duration_;
}


constexpr image_checksum
measured_render::checksum() const noexcept
{
  return checksum_;
}


constexpr benchmark_case const &
benchmark_result::benchmark() const noexcept
{
  return benchmark_;
}


constexpr std::vector<measured_render> const &
benchmark_result::renders() const noexcept
{
  return renders_;
}


constexpr duration_summary
benchmark_result::summary() const noexcept
{
  return summary_;
}


constexpr run_checksum
benchmark_result::checksum() const noexcept
{
  return checksum_;
}


constexpr std::optional<run_checksum>
benchmark_result::warmup_checksum() const noexcept
{
  return warmup_checksum_;
}


constexpr std::vector<image_checksum> const &
benchmark_result::warmup_checksums() const noexcept
{
  return warmup_checksums_;
}

} // namespace htracer::benchmarks

#endif
