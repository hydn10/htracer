#include <htracer_benchmarks/measurement_plan.hpp>

#include <cstdint>
#include <stdexcept>


namespace htracer::benchmarks
{

repetition_count
repetition_count::make(std::uint32_t value)
{
  if (value == 0)
  {
    throw std::invalid_argument("repetition count must be greater than zero");
  }
  return repetition_count{value};
}

} // namespace htracer::benchmarks
