#ifndef HTRACER_BENCHMARKS_CLI_COMMANDS_HPP
#define HTRACER_BENCHMARKS_CLI_COMMANDS_HPP


#include <htracer_benchmarks/cli/option_values.hpp>

#include <optional>


namespace htracer::benchmarks
{

struct list_command
{
};


struct quick_suite_command
{
  std::optional<output_option> output;
};


struct mixed_deterministic_command
{
  render_options render;
};


struct mixed_randomized_command
{
  randomized_options randomized;
  render_options render;
};


struct traversal_deterministic_command
{
  traversal_option traversal;
  render_options render;
};


struct traversal_randomized_command
{
  traversal_option traversal;
  randomized_options randomized;
  render_options render;
};


struct rng_probe_randomized_command
{
  randomized_options randomized;
  render_options render;
};

} // namespace htracer::benchmarks

#endif
