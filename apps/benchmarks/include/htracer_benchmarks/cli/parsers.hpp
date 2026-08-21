#ifndef HTRACER_BENCHMARKS_CLI_PARSERS_HPP
#define HTRACER_BENCHMARKS_CLI_PARSERS_HPP


#include <htracer/rendering/random_seed.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/image_extent.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>
#include <htracer_benchmarks/scene_spec.hpp>
#include <htracer_benchmarks/cli_structure/foundations/parse_result.hpp>

#include <filesystem>
#include <string_view>


namespace htracer::benchmarks::cli
{

struct width_parser
{
  [[nodiscard]]
  cli_structure::parse_result<image_width>
  operator()(std::string_view text) const;
};


struct height_parser
{
  [[nodiscard]]
  cli_structure::parse_result<image_height>
  operator()(std::string_view text) const;
};


struct precision_parser
{
  [[nodiscard]]
  cli_structure::parse_result<precision_kind>
  operator()(std::string_view text) const;
};


struct policy_parser
{
  [[nodiscard]]
  cli_structure::parse_result<policy_kind>
  operator()(std::string_view text) const;
};


struct warmups_parser
{
  [[nodiscard]]
  cli_structure::parse_result<warmup_count>
  operator()(std::string_view text) const;
};


struct repetitions_parser
{
  [[nodiscard]]
  cli_structure::parse_result<repetition_count>
  operator()(std::string_view text) const;
};


struct output_parser
{
  [[nodiscard]]
  cli_structure::parse_result<std::filesystem::path>
  operator()(std::string_view text) const;
};


struct traversal_parser
{
  [[nodiscard]]
  cli_structure::parse_result<traversal_scene>
  operator()(std::string_view text) const;
};


struct samples_parser
{
  [[nodiscard]]
  cli_structure::parse_result<htracer::rendering::samples_per_pixel>
  operator()(std::string_view text) const;
};


struct seed_parser
{
  [[nodiscard]]
  cli_structure::parse_result<htracer::rendering::random_seed>
  operator()(std::string_view text) const;
};


struct extent_parser
{
  [[nodiscard]]
  cli_structure::parse_result<image_extent>
  operator()(image_width width, image_height height) const;
};

} // namespace htracer::benchmarks::cli

#endif
