#include <htracer_benchmarks/cli/parsers/samples_parser.hpp>

#include <htracer_benchmarks/cli/parsers/detail/helpers.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>
#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>

#include <cstdint>
#include <string_view>


namespace htracer::benchmarks::cli::parsers
{

cli_structure::value_parsing::parse_result<htracer::rendering::samples_per_pixel>
samples_parser::operator()(std::string_view text) const
{
  return detail::parse_unsigned<std::uint32_t>(text).and_then(
      [](std::uint32_t value)
      {
        return detail::as_parse_result(htracer::rendering::samples_per_pixel::try_make(value));
      });
}

} // namespace htracer::benchmarks::cli::parsers
