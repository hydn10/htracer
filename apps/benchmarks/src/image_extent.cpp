#include <htracer_benchmarks/image_extent.hpp>

#include <cstddef>
#include <cstdint>
#include <expected>
#include <limits>
#include <string_view>


namespace htracer::benchmarks
{

std::expected<image_width, std::string_view>
image_width::try_make(std::uint32_t value) noexcept
{
  if (value == 0)
  {
    return std::unexpected{"image width must be greater than zero"};
  }
  return image_width{value};
}


std::expected<image_height, std::string_view>
image_height::try_make(std::uint32_t value) noexcept
{
  if (value == 0)
  {
    return std::unexpected{"image height must be greater than zero"};
  }
  return image_height{value};
}


std::expected<image_extent, std::string_view>
image_extent::try_make(image_width width, image_height height) noexcept
{
  if (width.value() > std::numeric_limits<std::uint32_t>::max() / height.value())
  {
    return std::unexpected{"image pixel count exceeds htracer's supported range"};
  }

  constexpr std::size_t largest_pixel_size = 3 * sizeof(double);
  auto const pixels = static_cast<std::size_t>(width.value()) * height.value();
  if (pixels > std::numeric_limits<std::size_t>::max() / largest_pixel_size)
  {
    return std::unexpected{"image storage size exceeds the addressable range"};
  }
  return image_extent{width, height};
}

} // namespace htracer::benchmarks
