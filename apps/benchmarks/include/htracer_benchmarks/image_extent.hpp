#ifndef HTRACER_BENCHMARKS_IMAGE_EXTENT_HPP
#define HTRACER_BENCHMARKS_IMAGE_EXTENT_HPP


#include <cstddef>
#include <cstdint>
#include <expected>
#include <limits>
#include <string_view>


namespace htracer::benchmarks
{

class image_width
{
  std::uint32_t value_;

  explicit constexpr image_width(std::uint32_t value) noexcept;

public:
  [[nodiscard]]
  static constexpr std::expected<image_width, std::string_view>
  try_make(std::uint32_t value) noexcept;

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept;
};


class image_height
{
  std::uint32_t value_;

  explicit constexpr image_height(std::uint32_t value) noexcept;

public:
  [[nodiscard]]
  static constexpr std::expected<image_height, std::string_view>
  try_make(std::uint32_t value) noexcept;

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept;
};


class image_extent
{
  image_width width_;
  image_height height_;

  constexpr image_extent(image_width width, image_height height) noexcept;

public:
  [[nodiscard]]
  static constexpr std::expected<image_extent, std::string_view>
  try_make(image_width width, image_height height) noexcept;

  [[nodiscard]]
  constexpr image_width
  width() const noexcept;

  [[nodiscard]]
  constexpr image_height
  height() const noexcept;

  [[nodiscard]]
  constexpr std::size_t
  pixel_count() const noexcept;
};


constexpr image_width::image_width(std::uint32_t value) noexcept
    : value_{value}
{
}


constexpr std::uint32_t
image_width::value() const noexcept
{
  return value_;
}


constexpr image_height::image_height(std::uint32_t value) noexcept
    : value_{value}
{
}


constexpr std::uint32_t
image_height::value() const noexcept
{
  return value_;
}


constexpr image_extent::image_extent(image_width width, image_height height) noexcept
    : width_{width}
    , height_{height}
{
}


constexpr image_width
image_extent::width() const noexcept
{
  return width_;
}


constexpr image_height
image_extent::height() const noexcept
{
  return height_;
}


constexpr std::size_t
image_extent::pixel_count() const noexcept
{
  return static_cast<std::size_t>(width_.value()) * height_.value();
}


constexpr std::expected<image_width, std::string_view>
image_width::try_make(std::uint32_t value) noexcept
{
  if (value == 0)
  {
    return std::unexpected{"image width must be greater than zero"};
  }

  return image_width{value};
}


constexpr std::expected<image_height, std::string_view>
image_height::try_make(std::uint32_t value) noexcept
{
  if (value == 0)
  {
    return std::unexpected{"image height must be greater than zero"};
  }

  return image_height{value};
}


constexpr std::expected<image_extent, std::string_view>
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

#endif
