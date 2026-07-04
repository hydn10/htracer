#ifndef HTRACER_BENCHMARKS_RUN_REPORT_HPP
#define HTRACER_BENCHMARKS_RUN_REPORT_HPP


#include <htracer_benchmarks/benchmark_result.hpp>

#include <string>
#include <vector>


namespace htracer::benchmarks
{

struct environment_info
{
  std::string os;
  std::string architecture;
  std::string compiler;
  std::string compiler_version;
  std::string build_type;
  unsigned logical_processors;
};


struct run_report
{
  environment_info environment;
  std::vector<benchmark_result> results;
};

} // namespace htracer::benchmarks

#endif
