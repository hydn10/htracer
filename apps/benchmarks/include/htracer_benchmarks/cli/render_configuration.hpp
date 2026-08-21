#ifndef HTRACER_BENCHMARKS_CLI_RENDER_CONFIGURATION_HPP
#define HTRACER_BENCHMARKS_CLI_RENDER_CONFIGURATION_HPP


#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/image_extent.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>

#include <filesystem>
#include <optional>


namespace htracer::benchmarks
{

struct render_configuration
{
  image_extent extent;
  precision_kind precision;
  policy_kind policy;
  measurement_plan measurement;
  std::optional<std::filesystem::path> output;
};

} // namespace htracer::benchmarks

#endif
