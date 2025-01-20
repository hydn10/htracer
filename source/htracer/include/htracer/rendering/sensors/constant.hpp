#ifndef HTRACER_RENDERING_SENSORS_CONSTANT_HPP
#define HTRACER_RENDERING_SENSORS_CONSTANT_HPP


#include <utility>


namespace htracer::rendering::sensors
{

template<typename Float>
class constant
{
public:
  std::pair<Float, Float>
  get_coords(uint32_t v_idx, uint32_t h_idx) const noexcept;
};


template<typename Float>
std::pair<Float, Float>
constant<Float>::get_coords(uint32_t v_idx, uint32_t h_idx) const noexcept
{
  return {v_idx, h_idx};
}

} // namespace htracer::rendering::sensors

#endif
