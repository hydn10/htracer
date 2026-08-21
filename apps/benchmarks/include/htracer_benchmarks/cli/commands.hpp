#ifndef HTRACER_BENCHMARKS_CLI_COMMANDS_HPP
#define HTRACER_BENCHMARKS_CLI_COMMANDS_HPP


#include <htracer_benchmarks/cli/render_configuration.hpp>
#include <htracer_benchmarks/render_mode.hpp>
#include <htracer_benchmarks/scene_spec.hpp>

#include <filesystem>
#include <optional>


namespace htracer::benchmarks
{

struct list_command
{
};


struct quick_suite_command
{
  std::optional<std::filesystem::path> output;
};


struct mixed_deterministic_command
{
  render_configuration render;
};


struct mixed_randomized_command
{
  randomized_render randomized;
  render_configuration render;
};


// Default construction is deleted because traversal_scene itself requires a validated geometry count.
// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
struct traversal_deterministic_command
{
  traversal_scene traversal;
  render_configuration render;
};


// Default construction is deleted because traversal_scene itself requires a validated geometry count.
// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
struct traversal_randomized_command
{
  traversal_scene traversal;
  randomized_render randomized;
  render_configuration render;
};


struct rng_probe_randomized_command
{
  randomized_render randomized;
  render_configuration render;
};

} // namespace htracer::benchmarks

#endif
