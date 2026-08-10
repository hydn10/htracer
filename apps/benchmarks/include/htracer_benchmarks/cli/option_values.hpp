#ifndef HTRACER_BENCHMARKS_CLI_OPTION_VALUES_HPP
#define HTRACER_BENCHMARKS_CLI_OPTION_VALUES_HPP


#include <htracer/rendering/random_seed.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/image_extent.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>

#include <filesystem>
#include <optional>


namespace htracer::benchmarks
{

struct width_option
{
  image_width value;
};


struct height_option
{
  image_height value;
};


struct extent_option
{
  image_extent value;
};


struct precision_option
{
  precision_kind value;
};


struct policy_option
{
  policy_kind value;
};


struct warmups_option
{
  warmup_count value;
};


struct repetitions_option
{
  repetition_count value;
};


struct output_option
{
  std::filesystem::path value;
};


struct traversal_option
{
  traversal_scene value;
};


struct samples_option
{
  htracer::rendering::samples_per_pixel value;
};


struct seed_option
{
  htracer::rendering::random_seed value;
};


struct render_options
{
  extent_option extent;
  precision_option precision;
  policy_option policy;
  warmups_option warmups;
  repetitions_option repetitions;
  std::optional<output_option> output;
};


struct randomized_options
{
  samples_option samples;
  std::optional<seed_option> seed;
};

} // namespace htracer::benchmarks

#endif
