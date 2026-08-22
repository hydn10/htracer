#include <htracer_benchmarks/cli/parsers/extent_parser.hpp>

#include <htracer_benchmarks/cli/parsers/detail/helpers.hpp>
#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>
#include <htracer_benchmarks/image_extent.hpp>


namespace htracer::benchmarks::cli::parsers
{

cli_structure::value_parsing::parse_result<image_extent>
extent_parser::operator()(image_width width, image_height height) const
{
  return detail::as_parse_result(image_extent::try_make(width, height));
}

} // namespace htracer::benchmarks::cli::parsers
