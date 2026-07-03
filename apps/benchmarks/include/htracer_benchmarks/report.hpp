#ifndef HTRACER_BENCHMARKS_REPORT_HPP
#define HTRACER_BENCHMARKS_REPORT_HPP


#include <htracer_benchmarks/model.hpp>

#include <filesystem>
#include <ostream>
#include <span>
#include <string_view>


namespace htracer::benchmarks
{

[[nodiscard]]
std::string_view
to_string(scene_kind value) noexcept;

[[nodiscard]]
std::string_view
to_string(rendering_kind value) noexcept;

[[nodiscard]]
std::string_view
to_string(precision_kind value) noexcept;

[[nodiscard]]
std::string_view
to_string(policy_kind value) noexcept;

[[nodiscard]]
std::string_view
to_string(batcher_kind value) noexcept;

[[nodiscard]]
std::string_view
to_string(sensor_kind value) noexcept;

[[nodiscard]]
std::string_view
to_string(lens_kind value) noexcept;


[[nodiscard]]
environment_info
get_environment_info();


void
print_case_list(std::ostream &output, std::span<benchmark_case const> benchmarks);


void
print_environment(std::ostream &output, environment_info const &environment);


void
print_result(std::ostream &output, benchmark_result const &result);


void
write_json(std::filesystem::path const &path, run_report const &report);

} // namespace htracer::benchmarks

#endif
