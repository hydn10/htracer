#ifndef HTRACER_BENCHMARKS_IMAGE_EXTENT_HPP
#define HTRACER_BENCHMARKS_IMAGE_EXTENT_HPP


#include <cstddef>
#include <cstdint>


namespace htracer::benchmarks
{

class image_width
{
public:
  [[nodiscard]]
  static image_width
  make(std::uint32_t value);

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept
  {
    return value_;
  }

private:
  explicit constexpr image_width(std::uint32_t value) noexcept
      : value_{value}
  {
  }

  std::uint32_t value_;
};


class image_height
{
public:
  [[nodiscard]]
  static image_height
  make(std::uint32_t value);

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept
  {
    return value_;
  }

private:
  explicit constexpr image_height(std::uint32_t value) noexcept
      : value_{value}
  {
  }

  std::uint32_t value_;
};


class image_extent
{
public:
  [[nodiscard]]
  static image_extent
  make(image_width width, image_height height);

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

private:
  constexpr image_extent(image_width width, image_height height) noexcept
      : width_{width}
      , height_{height}
  {
  }

  image_width width_;
  image_height height_;
};

} // namespace htracer::benchmarks

#endif
