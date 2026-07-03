#ifndef HTRACER_BENCHMARKS_REPORT_HPP
#define HTRACER_BENCHMARKS_REPORT_HPP


#include <htracer_benchmarks/model.hpp>

#include <filesystem>
#include <span>
#include <string>
#include <string_view>


namespace htracer::benchmarks
{

[[nodiscard]]
std::string
benchmark_name(benchmark_case const &benchmark);


[[nodiscard]]
environment_info
get_environment_info();


void
print_case_list(std::span<benchmark_case const> benchmarks);


void
print_environment(environment_info const &environment);


void
print_result(benchmark_result const &result);


void
write_json(std::filesystem::path const &path, run_report const &report);

} // namespace htracer::benchmarks

#endif
