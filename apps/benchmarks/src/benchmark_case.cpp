#include <htracer_benchmarks/benchmark_case.hpp>

#include <htracer_benchmarks/benchmark_definition.hpp>

namespace htracer::benchmarks
{

benchmark_case::benchmark_case(origin provenance, benchmark_definition definition)
    : origin_{provenance}
    , definition_{definition}
{
}


benchmark_case
benchmark_case::canonical(benchmark_definition definition)
{
  return {origin::canonical, definition};
}


benchmark_case
benchmark_case::custom(benchmark_definition definition)
{
  return {origin::custom, definition};
}

} // namespace htracer::benchmarks
