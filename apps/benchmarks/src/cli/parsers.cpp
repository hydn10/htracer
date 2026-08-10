#include <htracer_benchmarks/cli/parsers.hpp>

#include <htracer/rendering/random_seed.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/cli/option_values.hpp>
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


cli_structure::parse_result<width_option>
width_parser::operator()(std::string_view text) const
{
  return parse_unsigned<std::uint32_t>(text)
      .and_then([](std::uint32_t value) {
    return as_parse_result(image_width::try_make(value));
  }).transform([](image_width value) { return width_option{value}; });
}


cli_structure::parse_result<height_option>
height_parser::operator()(std::string_view text) const
{
  return parse_unsigned<std::uint32_t>(text)
      .and_then([](std::uint32_t value) {
    return as_parse_result(image_height::try_make(value));
  }).transform([](image_height value) { return height_option{value}; });
}


cli_structure::parse_result<precision_option>
precision_parser::operator()(std::string_view text) const
{
  if (text == "float")
  {
    return precision_option{precision_kind::f32};
  }
  if (text == "double")
  {
    return precision_option{precision_kind::f64};
  }
  return std::unexpected{cli_structure::parse_error{"expected 'float' or 'double'"}};
}


cli_structure::parse_result<policy_option>
policy_parser::operator()(std::string_view text) const
{
  if (text == "seq")
  {
    return policy_option{policy_kind::seq};
  }
  if (text == "par")
  {
    return policy_option{policy_kind::par};
  }
  return std::unexpected{cli_structure::parse_error{"expected 'seq' or 'par'"}};
}


cli_structure::parse_result<warmups_option>
warmups_parser::operator()(std::string_view text) const
{
  return parse_unsigned<std::uint32_t>(text).transform([](std::uint32_t value)
  { return warmups_option{warmup_count{value}}; });
}


cli_structure::parse_result<repetitions_option>
repetitions_parser::operator()(std::string_view text) const
{
  return parse_unsigned<std::uint32_t>(text)
      .and_then([](std::uint32_t value) {
    return as_parse_result(repetition_count::try_make(value));
  }).transform([](repetition_count value) { return repetitions_option{value}; });
}


cli_structure::parse_result<output_option>
output_parser::operator()(std::string_view text) const
{
  return output_option{std::filesystem::path{std::string{text}}};
}


cli_structure::parse_result<traversal_option>
traversal_parser::operator()(std::string_view text) const
{
  return parse_unsigned<std::uint32_t>(text)
      .and_then([](std::uint32_t value) {
    return as_parse_result(geometry_count::try_make(value));
  }).transform([](geometry_count value) { return traversal_option{traversal_scene{value}}; });
}


cli_structure::parse_result<samples_option>
samples_parser::operator()(std::string_view text) const
{
  return parse_unsigned<std::uint32_t>(text)
      .and_then([](std::uint32_t value) {
    return as_parse_result(htracer::rendering::samples_per_pixel::try_make(value));
  }).transform([](htracer::rendering::samples_per_pixel value) { return samples_option{value}; });
}


cli_structure::parse_result<seed_option>
seed_parser::operator()(std::string_view text) const
{
  return parse_unsigned<std::uint64_t>(text).transform([](std::uint64_t value)
  { return seed_option{htracer::rendering::random_seed{value}}; });
}


cli_structure::parse_result<extent_option>
extent_parser::operator()(width_option width, height_option height) const
{
  return as_parse_result(image_extent::try_make(width.value, height.value)).transform([](image_extent value) {
    return extent_option{value};
  });
}

} // namespace htracer::benchmarks::cli
