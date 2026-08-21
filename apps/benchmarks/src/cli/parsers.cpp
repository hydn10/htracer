#include <htracer_benchmarks/cli/parsers.hpp>

#include <htracer/rendering/random_seed.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/cli_structure/foundations/parse_result.hpp>
#include <htracer_benchmarks/cli_structure/parsing/parsers.hpp>
#include <htracer_benchmarks/image_extent.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>
#include <htracer_benchmarks/scene_spec.hpp>

#include <cstdint>
#include <expected>
#include <filesystem>
#include <string>
#include <string_view>
#include <utility>


namespace htracer::benchmarks::cli
{
namespace
{

template<typename Value>
[[nodiscard]]
cli_structure::parse_result<Value>
parse_unsigned(std::string_view text)
{
  return cli_structure::unsigned_integer<Value>{}(text);
}


template<typename Value>
[[nodiscard]]
cli_structure::parse_result<Value>
as_parse_result(std::expected<Value, std::string_view> result)
{
  if (!result)
  {
    return std::unexpected{cli_structure::parse_error{std::string{result.error()}}};
  }

  return std::move(*result);
}

} // namespace


cli_structure::parse_result<image_width>
width_parser::operator()(std::string_view text) const
{
  return parse_unsigned<std::uint32_t>(text).and_then(
      [](std::uint32_t value)
      {
        return as_parse_result(image_width::try_make(value));
      });
}


cli_structure::parse_result<image_height>
height_parser::operator()(std::string_view text) const
{
  return parse_unsigned<std::uint32_t>(text).and_then(
      [](std::uint32_t value)
      {
        return as_parse_result(image_height::try_make(value));
      });
}


cli_structure::parse_result<precision_kind>
precision_parser::operator()(std::string_view text) const
{
  if (text == "float")
  {
    return precision_kind::f32;
  }
  if (text == "double")
  {
    return precision_kind::f64;
  }

  return std::unexpected{cli_structure::parse_error{"expected 'float' or 'double'"}};
}


cli_structure::parse_result<policy_kind>
policy_parser::operator()(std::string_view text) const
{
  if (text == "seq")
  {
    return policy_kind::seq;
  }
  if (text == "par")
  {
    return policy_kind::par;
  }

  return std::unexpected{cli_structure::parse_error{"expected 'seq' or 'par'"}};
}


cli_structure::parse_result<warmup_count>
warmups_parser::operator()(std::string_view text) const
{
  return parse_unsigned<std::uint32_t>(text).transform(
      [](std::uint32_t value)
      {
        return warmup_count{value};
      });
}


cli_structure::parse_result<repetition_count>
repetitions_parser::operator()(std::string_view text) const
{
  return parse_unsigned<std::uint32_t>(text).and_then(
      [](std::uint32_t value)
      {
        return as_parse_result(repetition_count::try_make(value));
      });
}


cli_structure::parse_result<std::filesystem::path>
output_parser::operator()(std::string_view text) const
{
  return std::filesystem::path{std::string{text}};
}


cli_structure::parse_result<traversal_scene>
traversal_parser::operator()(std::string_view text) const
{
  return parse_unsigned<std::uint32_t>(text).and_then(
      [](std::uint32_t value)
      {
        return as_parse_result(geometry_count::try_make(value));
      }).transform(
      [](geometry_count value)
      {
        return traversal_scene{value};
      });
}


cli_structure::parse_result<htracer::rendering::samples_per_pixel>
samples_parser::operator()(std::string_view text) const
{
  return parse_unsigned<std::uint32_t>(text).and_then(
      [](std::uint32_t value)
      {
        return as_parse_result(htracer::rendering::samples_per_pixel::try_make(value));
      });
}


cli_structure::parse_result<htracer::rendering::random_seed>
seed_parser::operator()(std::string_view text) const
{
  return parse_unsigned<std::uint64_t>(text).transform(
      [](std::uint64_t value)
      {
        return htracer::rendering::random_seed{value};
      });
}


cli_structure::parse_result<image_extent>
extent_parser::operator()(image_width width, image_height height) const
{
  return as_parse_result(image_extent::try_make(width, height));
}

} // namespace htracer::benchmarks::cli
