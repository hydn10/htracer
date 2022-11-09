#ifndef HTRACER_RAYTRACING_PIXEL_SAMPLERS_CONSTANT_HPP
#define HTRACER_RAYTRACING_PIXEL_SAMPLERS_CONSTANT_HPP


#include <utility>


namespace htracer::raytracing::pixel_samplers
{

template<typename Float>
class constant
{
public:
  std::pair<Float, Float>
  get_coords(uint32_t v_idx, uint32_t h_idx) const
  {
    return {v_idx, h_idx};
  }
};

} // namespace htracer::raytracing::pixel_samplers

#endif