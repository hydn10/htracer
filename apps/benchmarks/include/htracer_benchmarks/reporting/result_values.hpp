#ifndef HTRACER_BENCHMARKS_REPORTING_RESULT_VALUES_HPP
#define HTRACER_BENCHMARKS_REPORTING_RESULT_VALUES_HPP


#include <htracer_benchmarks/benchmark_result.hpp>
#include <htracer_benchmarks/render_mode.hpp>

#include <cstdint>
#include <string>


namespace htracer::benchmarks::reporting
{

[[nodiscard]]
std::uint32_t
effective_samples(render_mode const &rendering);


[[nodiscard]]
long double
pixels_per_second(benchmark_result const &result);


[[nodiscard]]
long double
primary_samples_per_second(benchmark_result const &result);


[[nodiscard]]
std::string
hex_checksum(image_checksum checksum);

} // namespace htracer::benchmarks::reporting

#endif
