#ifndef HTRACER_BENCHMARKS_BENCHMARK_CASE_HPP
#define HTRACER_BENCHMARKS_BENCHMARK_CASE_HPP


#include <htracer_benchmarks/benchmark_definition.hpp>


namespace htracer::benchmarks
{

class quick_suite_catalog;


class benchmark_case
{
  friend class quick_suite_catalog;

  enum class origin
  {
    canonical,
    custom
  };

  [[nodiscard]]
  static benchmark_case
  canonical(benchmark_definition definition);

  benchmark_case(origin provenance, benchmark_definition definition);

  origin origin_;
  benchmark_definition definition_;

public:
  [[nodiscard]]
  static benchmark_case
  custom(benchmark_definition definition);

  [[nodiscard]]
  constexpr benchmark_definition const &
  definition() const noexcept
  {
    return definition_;
  }

  [[nodiscard]]
  constexpr bool
  is_canonical() const noexcept
  {
    return origin_ == origin::canonical;
  }
};

} // namespace htracer::benchmarks

#endif
