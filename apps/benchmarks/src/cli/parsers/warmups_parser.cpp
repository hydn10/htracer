#include <htracer_benchmarks/cli/parsers/warmups_parser.hpp>

#include <htracer_benchmarks/cli/parsers/detail/helpers.hpp>
#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>

#include <cstdint>
#include <string_view>


namespace htracer::benchmarks::cli::parsers
{

cli_structure::value_parsing::parse_result<warmup_count>
warmups_parser::operator()(std::string_view text) const
{
  return detail::parse_unsigned<std::uint32_t>(text).transform(
      [](std::uint32_t value)
      {
        return warmup_count{value};
      });
}

} // namespace htracer::benchmarks::cli::parsers
