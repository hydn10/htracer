#include <htracer_benchmarks/image_extent.hpp>

#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>


namespace htracer::benchmarks
{

image_width
image_width::make(std::uint32_t value)
{
  if (value == 0)
  {
    throw std::invalid_argument("image width must be greater than zero");
  }
  return image_width{value};
}


image_height
image_height::make(std::uint32_t value)
{
  if (value == 0)
  {
    throw std::invalid_argument("image height must be greater than zero");
  }
  return image_height{value};
}


image_extent
image_extent::make(image_width width, image_height height)
{
  if (width.value() > std::numeric_limits<std::uint32_t>::max() / height.value())
  {
    throw std::invalid_argument("image pixel count exceeds htracer's supported range");
  }

  constexpr std::size_t largest_pixel_size = 3 * sizeof(double);
  auto const pixels = static_cast<std::size_t>(width.value()) * height.value();
  if (pixels > std::numeric_limits<std::size_t>::max() / largest_pixel_size)
  {
    throw std::invalid_argument("image storage size exceeds the addressable range");
  }
  return image_extent{width, height};
}

} // namespace htracer::benchmarks
