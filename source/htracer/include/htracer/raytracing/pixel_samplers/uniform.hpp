#ifndef HTRACER_RAYTRACING_PIXEL_SAMPLERS_UNIFORM_HPP
#define HTRACER_RAYTRACING_PIXEL_SAMPLERS_UNIFORM_HPP


#include <htracer/utils/randomness.hpp>

#include <random>
#include <utility>


namespace htracer::raytracing::pixel_samplers
{

template<typename Float>
class uniform
{
  std::uniform_real_distribution<Float> pixel_limits_dist_;

public:
  uniform()
      : pixel_limits_dist_{-.5, .5}
  {
  }

  template<typename Engine>
  std::pair<Float, Float>
  get_coords(uint32_t v_idx, uint32_t h_idx, htracer::utils::randomness<Engine> &rand);
};


template<typename Float>
template<typename Engine>
std::pair<Float, Float>
uniform<Float>::get_coords(uint32_t v_idx, uint32_t h_idx, htracer::utils::randomness<Engine> &rand)
{
  auto const rand_offset = [&]()
  {
    return rand(pixel_limits_dist_);
  };

  return {v_idx + rand_offset(), h_idx + rand_offset()};
}

} // namespace htracer::raytracing::pixel_samplers

#endif
