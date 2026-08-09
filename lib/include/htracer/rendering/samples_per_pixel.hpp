#ifndef HTRACER_RENDERING_SAMPLES_PER_PIXEL_HPP
#define HTRACER_RENDERING_SAMPLES_PER_PIXEL_HPP


#include <cstdint>
#include <stdexcept>


namespace htracer::rendering
{

class samples_per_pixel
{
public:
  explicit constexpr samples_per_pixel(std::uint32_t value)
      : value_{value}
  {
    if (value == 0)
    {
      throw std::invalid_argument("samples per pixel must be greater than zero");
    }
  }

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept
  {
    return value_;
  }

private:
  std::uint32_t value_;
};

} // namespace htracer::rendering

#endif
