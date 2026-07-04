#ifndef HTRACER_BENCHMARKS_REPORTING_CONSOLE_HPP
#define HTRACER_BENCHMARKS_REPORTING_CONSOLE_HPP


#include <htracer_benchmarks/benchmark_case.hpp>
#include <htracer_benchmarks/benchmark_result.hpp>
#include <htracer_benchmarks/run_report.hpp>

#include <span>


namespace htracer::benchmarks::reporting
{

void
print_case_list(std::span<benchmark_case const> benchmarks);


void
print_environment(environment_info const &environment);


void
print_result(benchmark_result const &result);

} // namespace htracer::benchmarks::reporting

#endif
