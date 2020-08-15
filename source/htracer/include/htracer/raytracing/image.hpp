#ifndef HTRACER_RAYTRACING_IMAGE_HPP
#define HTRACER_RAYTRACING_IMAGE_HPP


#include <htracer/colors/colors.hpp>

#include <vector>


namespace htracer::raytracing
{
template<typename Float>
class image
{
  const std::vector<colors::srgb_linear<Float>> pixels_;
  const uint32_t h_res_;
  const uint32_t v_res_;

public:
  image(
      uint32_t h_res,
      uint32_t v_res,
      std::vector<colors::srgb_linear<Float>> pixels);

  [[nodiscard]] uint32_t
  h_res() const;
  [[nodiscard]] uint32_t
  v_res() const;

  [[nodiscard]] std::vector<colors::srgb_linear<Float>> const&
  pixels() const;
};


template<typename Float>
image<Float>::image(
    uint32_t h_res,
    uint32_t v_res,
    std::vector<colors::srgb_linear<Float>> pixels)
    : h_res_{h_res}, v_res_{v_res}, pixels_(std::move(pixels))
{
  // TODO: Assert pixels_.size() == h_res_ * v_res_
}


template<typename Float>
uint32_t
image<Float>::h_res() const
{
  return h_res_;
}


template<typename Float>
uint32_t
image<Float>::v_res() const
{
  return v_res_;
}


template<typename Float>
std::vector<colors::srgb_linear<Float>> const&
image<Float>::pixels() const
{
  return pixels_;
}

} // namespace htracer::raytracing

#endif // HTRACER_RAYTRACING_IMAGE_HPP
