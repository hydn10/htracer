#ifndef HTRACER_RAYTRACING_PIXEL_SAMPLERS_UNIFORM_HPP
#define HTRACER_RAYTRACING_PIXEL_SAMPLERS_UNIFORM_HPP


#include <random>
#include <utility>


namespace htracer::rendering::sensors
{

template<typename Float>
class uniform
{
public:
  template<std::uniform_random_bit_generator Generator>
  std::pair<Float, Float>
  get_coords(uint32_t v_idx, uint32_t h_idx, Generator &generator) const;
};


template<typename Float>
template<std::uniform_random_bit_generator Generator>
std::pair<Float, Float>
uniform<Float>::get_coords(uint32_t v_idx, uint32_t h_idx, Generator &generator) const
{
  std::uniform_real_distribution<Float> pixel_limits_dist{-.5, .5};

  auto const rand_offset = [&pixel_limits_dist, &generator]()
  {
    return pixel_limits_dist(generator);
  };

  return {v_idx + rand_offset(), h_idx + rand_offset()};
}

} // namespace htracer::rendering::sensors

#endif
