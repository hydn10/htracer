#ifndef HTRACER_BENCHMARKS_RENDER_BENCHMARK_HPP
#define HTRACER_BENCHMARKS_RENDER_BENCHMARK_HPP


#include <htracer_benchmarks/benchmark_case.hpp>
#include <htracer_benchmarks/benchmark_result.hpp>


namespace htracer::benchmarks
{

[[nodiscard]]
benchmark_result
run_benchmark(benchmark_case const &benchmark);

} // namespace htracer::benchmarks

#endif
