#include <htracer_benchmarks/cli.hpp>

#include <htracer_benchmarks/cli/custom_definition.hpp>
#include <htracer_benchmarks/cli/raw_options.hpp>
#include <htracer_benchmarks/invocation.hpp>

#include <filesystem>
#include <optional>
#include <print>
#include <span>


namespace htracer::benchmarks
{

invocation
parse_cli(std::span<char const *const> arguments)
{
  auto const options = cli::parse_raw_options(arguments);
  auto const operation_count = static_cast<unsigned>(options.help) + static_cast<unsigned>(options.list) +
                               static_cast<unsigned>(options.suite.has_value()) +
                               static_cast<unsigned>(options.benchmark.has_value());
  cli::require(operation_count == 1, "specify exactly one of --help, --list, --suite, or --benchmark");

  if (options.help)
  {
    cli::require(!options.output && !cli::has_custom_options(options), "--help does not accept additional options");
    return help_command{};
  }
  if (options.list)
  {
    cli::require(!options.output && !cli::has_custom_options(options), "--list does not accept additional options");
    return list_command{};
  }

  auto const output = options.output ? std::optional{std::filesystem::path{*options.output}} : std::nullopt;
  if (options.suite)
  {
    cli::require(*options.suite == "quick", "--suite only supports: quick");
    cli::require(!cli::has_custom_options(options), "--suite quick only accepts --output");
    return quick_suite_command{.output = output};
  }

  return custom_render_command{.benchmark = cli::parse_custom_definition(options), .output = output};
}


void
print_help()
{
  std::print(
      "Usage:\n"
      "  htracer-benchmarks --help\n"
      "  htracer-benchmarks --list\n"
      "  htracer-benchmarks --suite quick [--output PATH]\n"
      "  htracer-benchmarks --benchmark render OPTIONS\n\n"
      "Required custom render options:\n"
      "  --scene mixed|traversal|rng-probe\n"
      "  --rendering deterministic|randomized\n"
      "  --width N --height N\n"
      "  --precision float|double\n"
      "  --policy seq|par\n\n"
      "Conditional and measurement options:\n"
      "  --geometry-count N  Required only for traversal\n"
      "  --samples N         Required only for randomized rendering\n"
      "  --seed none|N       Optional unsigned decimal seed for randomized rendering\n"
      "  --warmup N          Default: 1; zero is allowed\n"
      "  --repetitions N     Default: 9\n"
      "  --output PATH       Write versioned JSON in addition to console output\n");
}

} // namespace htracer::benchmarks
