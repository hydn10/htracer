#ifndef HTRACER_RAYTRACING_PIXEL_SAMPLERS_UNIFORM_HPP
#define HTRACER_RAYTRACING_PIXEL_SAMPLERS_UNIFORM_HPP


#include <htracer/utils/random.hpp>

#include <utility>


namespace htracer::raytracing::pixel_samplers
{

template<typename Float>
class uniform
{
  utils::random<Float> rand_offset_;

public:
  uniform()
      : rand_offset_{-.5, .5}
  {
  }

  std::pair<Float, Float>
  get_coords(uint32_t v_idx, uint32_t h_idx)
  {
    return {v_idx + rand_offset_(), h_idx + rand_offset_()};
  }
};

} // namespace htracer::raytracing::pixel_samplers

#endif