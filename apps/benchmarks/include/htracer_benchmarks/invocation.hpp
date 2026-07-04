#ifndef HTRACER_BENCHMARKS_INVOCATION_HPP
#define HTRACER_BENCHMARKS_INVOCATION_HPP


#include <htracer_benchmarks/benchmark_definition.hpp>

#include <filesystem>
#include <optional>
#include <variant>


namespace htracer::benchmarks
{

struct help_command
{
};


struct list_command
{
};


struct quick_suite_command
{
  std::optional<std::filesystem::path> output;
};


struct custom_render_command
{
  benchmark_definition benchmark;
  std::optional<std::filesystem::path> output;
};


using invocation = std::variant<help_command, list_command, quick_suite_command, custom_render_command>;

} // namespace htracer::benchmarks

#endif
