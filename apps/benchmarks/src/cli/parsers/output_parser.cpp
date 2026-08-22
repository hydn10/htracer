#include <htracer_benchmarks/cli/parsers/output_parser.hpp>

#include <htracer_benchmarks/cli_structure/value_parsing/parse_result.hpp>

#include <filesystem>
#include <string>
#include <string_view>


namespace htracer::benchmarks::cli::parsers
{

cli_structure::value_parsing::parse_result<std::filesystem::path>
output_parser::operator()(std::string_view text) const
{
  return std::filesystem::path{std::string{text}};
}

} // namespace htracer::benchmarks::cli::parsers
