#include <htracer_benchmarks/scene_spec.hpp>

#include <cstdint>
#include <stdexcept>


namespace htracer::benchmarks
{

geometry_count
geometry_count::make(std::uint32_t value)
{
  if (value == 0)
  {
    throw std::invalid_argument("geometry count must be greater than zero");
  }
  return geometry_count{value};
}

} // namespace htracer::benchmarks
