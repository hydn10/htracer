#ifndef HTRACER_BENCHMARKS_REPORTING_BENCHMARK_NAME_HPP
#define HTRACER_BENCHMARKS_REPORTING_BENCHMARK_NAME_HPP


#include <htracer_benchmarks/benchmark_case.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/render_mode.hpp>
#include <htracer_benchmarks/scene_spec.hpp>

#include <string>
#include <string_view>


namespace htracer::benchmarks::reporting
{

[[nodiscard]]
std::string_view
scene_name(scene_spec const &scene);


[[nodiscard]]
std::string_view
rendering_name(render_mode const &rendering);


[[nodiscard]]
std::string_view
precision_name(precision_kind precision) noexcept;


[[nodiscard]]
std::string_view
policy_name(policy_kind policy) noexcept;


[[nodiscard]]
std::string
benchmark_name(benchmark_case const &benchmark);

} // namespace htracer::benchmarks::reporting

#endif
