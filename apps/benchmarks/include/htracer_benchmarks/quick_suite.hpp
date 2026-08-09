#ifndef HTRACER_BENCHMARKS_QUICK_SUITE_HPP
#define HTRACER_BENCHMARKS_QUICK_SUITE_HPP


#include <htracer_benchmarks/benchmark_case.hpp>

#include <span>


namespace htracer::benchmarks
{

class quick_suite_catalog
{
public:
  [[nodiscard]]
  static std::span<benchmark_case const>
  cases();
};

} // namespace htracer::benchmarks

#endif
