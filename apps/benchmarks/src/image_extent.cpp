#include <htracer_benchmarks/image_extent.hpp>

#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>


namespace htracer::benchmarks
{

image_extent
image_extent::make(std::uint32_t width, std::uint32_t height)
{
  if (width == 0 || height == 0)
  {
    throw std::invalid_argument("image dimensions must be greater than zero");
  }
  if (width > std::numeric_limits<std::uint32_t>::max() / height)
  {
    throw std::invalid_argument("image pixel count exceeds htracer's supported range");
  }

  constexpr std::size_t largest_pixel_size = 3 * sizeof(double);
  auto const pixels = static_cast<std::size_t>(width) * height;
  if (pixels > std::numeric_limits<std::size_t>::max() / largest_pixel_size)
  {
    throw std::invalid_argument("image storage size exceeds the addressable range");
  }
  return image_extent{width, height};
}

} // namespace htracer::benchmarks
