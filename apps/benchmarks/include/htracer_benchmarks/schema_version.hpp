#ifndef HTRACER_BENCHMARKS_SCHEMA_VERSION_HPP
#define HTRACER_BENCHMARKS_SCHEMA_VERSION_HPP


#include <cstdint>


namespace htracer::benchmarks
{

inline constexpr std::uint32_t output_schema_version{1};
inline constexpr std::uint32_t workload_schema_version{1};

} // namespace htracer::benchmarks

#endif
