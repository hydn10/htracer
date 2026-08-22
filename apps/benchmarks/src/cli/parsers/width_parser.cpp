#include <htracer_benchmarks/cli/parsers/width_parser.hpp>

#include <htracer_benchmarks/cli/parsers/detail/helpers.hpp>
#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>
#include <htracer_benchmarks/image_extent.hpp>

#include <cstdint>
#include <string_view>


namespace htracer::benchmarks::cli::parsers
{

cli_structure::value_parsing::parse_result<image_width>
width_parser::operator()(std::string_view text) const
{
  return detail::parse_unsigned<std::uint32_t>(text).and_then(
      [](std::uint32_t value)
      {
        return detail::as_parse_result(image_width::try_make(value));
      });
}

} // namespace htracer::benchmarks::cli::parsers
