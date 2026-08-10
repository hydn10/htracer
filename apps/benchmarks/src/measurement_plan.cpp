#include <htracer_benchmarks/measurement_plan.hpp>

#include <cstdint>
#include <expected>
#include <string_view>


namespace htracer::benchmarks
{

std::expected<repetition_count, std::string_view>
repetition_count::try_make(std::uint32_t value) noexcept
{
  if (value == 0)
  {
    return std::unexpected{"repetition count must be greater than zero"};
  }
  return repetition_count{value};
}

} // namespace htracer::benchmarks
