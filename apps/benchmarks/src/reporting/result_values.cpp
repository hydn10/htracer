#include <htracer_benchmarks/reporting/result_values.hpp>

#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/benchmark_result.hpp>
#include <htracer_benchmarks/render_mode.hpp>

#include <concepts>
#include <cstdint>
#include <format>
#include <string>
#include <variant>


namespace htracer::benchmarks::reporting
{

std::uint32_t
effective_samples(render_mode const &rendering)
{
  return std::visit(
      []<typename Mode>(Mode const &mode)
      {
        if constexpr (std::same_as<Mode, deterministic_render>)
        {
          return std::uint32_t{1};
        }
        else
        {
          static_assert(std::same_as<Mode, randomized_render>);
          return mode.samples().value();
        }
      },
      rendering);
}


long double
pixels_per_second(benchmark_result const &result)
{
  auto const &definition = result.benchmark().definition();
  auto const pixels = static_cast<long double>(definition.extent().pixel_count());
  auto const seconds = static_cast<long double>(result.summary().median.count()) / 1'000'000'000.0l;
  return pixels / seconds;
}


long double
primary_samples_per_second(benchmark_result const &result)
{
  return pixels_per_second(result) * effective_samples(result.benchmark().definition().rendering());
}


std::string
hex_checksum(run_checksum checksum)
{
  return std::format("0x{:016x}", checksum.value());
}

} // namespace htracer::benchmarks::reporting
