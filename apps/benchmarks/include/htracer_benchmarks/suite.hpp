#ifndef HTRACER_BENCHMARKS_SUITE_HPP
#define HTRACER_BENCHMARKS_SUITE_HPP


#include <htracer_benchmarks/model.hpp>

#include <vector>


namespace htracer::benchmarks
{

inline constexpr std::uint32_t output_schema_version{1};
inline constexpr std::uint32_t workload_schema_version{1};


[[nodiscard]]
std::vector<benchmark_case>
make_quick_suite();

} // namespace htracer::benchmarks

#endif
