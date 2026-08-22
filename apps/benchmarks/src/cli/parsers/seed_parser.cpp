#include <htracer_benchmarks/cli/parsers/seed_parser.hpp>

#include <htracer_benchmarks/cli/parsers/detail/helpers.hpp>
#include <htracer/rendering/random_seed.hpp>
#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>

#include <cstdint>
#include <string_view>


namespace htracer::benchmarks::cli::parsers
{

cli_structure::value_parsing::parse_result<htracer::rendering::random_seed>
seed_parser::operator()(std::string_view text) const
{
  return detail::parse_unsigned<std::uint64_t>(text).transform(
      [](std::uint64_t value)
      {
        return htracer::rendering::random_seed{value};
      });
}

} // namespace htracer::benchmarks::cli::parsers
