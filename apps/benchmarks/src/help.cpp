#include <htracer_benchmarks/cli.hpp>

#include <print>


namespace htracer::benchmarks
{

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
