#ifndef HTRACER_BENCHMARKS_IMAGE_EXTENT_HPP
#define HTRACER_BENCHMARKS_IMAGE_EXTENT_HPP


#include <cstddef>
#include <cstdint>
#include <expected>
#include <string_view>


namespace htracer::benchmarks
{

class image_width
{
  std::uint32_t value_;

  explicit constexpr image_width(std::uint32_t value) noexcept
      : value_{value}
  {
  }

public:
  [[nodiscard]]
  static std::expected<image_width, std::string_view>
  try_make(std::uint32_t value) noexcept;

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept
  {
    return value_;
  }
};


class image_height
{
  std::uint32_t value_;

  explicit constexpr image_height(std::uint32_t value) noexcept
      : value_{value}
  {
  }

public:
  [[nodiscard]]
  static std::expected<image_height, std::string_view>
  try_make(std::uint32_t value) noexcept;

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept
  {
    return value_;
  }
};


class image_extent
{
  image_width width_;
  image_height height_;

  constexpr image_extent(image_width width, image_height height) noexcept
      : width_{width}
      , height_{height}
  {
  }

public:
  [[nodiscard]]
  static std::expected<image_extent, std::string_view>
  try_make(image_width width, image_height height) noexcept;

  [[nodiscard]]
  constexpr image_width
  width() const noexcept
  {
    return width_;
  }

  [[nodiscard]]
  constexpr image_height
  height() const noexcept
  {
    return height_;
  }

  [[nodiscard]]
  constexpr std::size_t
  pixel_count() const noexcept
  {
    return static_cast<std::size_t>(width_.value()) * height_.value();
  }
};

} // namespace htracer::benchmarks

#endif
