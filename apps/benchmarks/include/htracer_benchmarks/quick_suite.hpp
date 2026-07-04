#ifndef HTRACER_BENCHMARKS_QUICK_SUITE_HPP
#define HTRACER_BENCHMARKS_QUICK_SUITE_HPP


#include <htracer_benchmarks/benchmark_case.hpp>

#include <cstdint>
#include <span>


namespace htracer::benchmarks
{

inline constexpr std::uint32_t output_schema_version{1};
inline constexpr std::uint32_t workload_schema_version{1};


class quick_suite_catalog
{
public:
  [[nodiscard]]
  static std::span<benchmark_case const>
  cases();
};

} // namespace htracer::benchmarks

#endif
