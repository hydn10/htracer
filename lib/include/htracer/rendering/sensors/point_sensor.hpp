#ifndef HTRACER_RENDERING_SENSORS_POINT_SENSOR_HPP
#define HTRACER_RENDERING_SENSORS_POINT_SENSOR_HPP


#include <utility>


namespace htracer::rendering::sensors
{

template<typename Float>
class point_sensor
{
public:
  constexpr std::pair<Float, Float>
  get_coords(uint32_t v_idx, uint32_t h_idx) const noexcept;
};


template<typename Float>
constexpr std::pair<Float, Float>
point_sensor<Float>::get_coords(uint32_t v_idx, uint32_t h_idx) const noexcept
{
  return {v_idx, h_idx};
}

} // namespace htracer::rendering::sensors

#endif
