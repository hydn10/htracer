#ifndef HTRACER_BENCHMARKS_REPORTING_JSON_HPP
#define HTRACER_BENCHMARKS_REPORTING_JSON_HPP


#include <htracer_benchmarks/run_report.hpp>

#include <filesystem>


namespace htracer::benchmarks::reporting
{

void
write_json(std::filesystem::path const &path, run_report const &report);

} // namespace htracer::benchmarks::reporting

#endif
