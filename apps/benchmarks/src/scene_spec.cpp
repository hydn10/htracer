#include <htracer_benchmarks/scene_spec.hpp>

#include <cstdint>
#include <expected>
#include <string_view>


namespace htracer::benchmarks
{

std::expected<geometry_count, std::string_view>
geometry_count::try_make(std::uint32_t value) noexcept
{
  if (value == 0)
  {
    return std::unexpected{"geometry count must be greater than zero"};
  }
  return geometry_count{value};
}

} // namespace htracer::benchmarks
