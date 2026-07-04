#ifndef HTRACER_BENCHMARKS_IMAGE_EXTENT_HPP
#define HTRACER_BENCHMARKS_IMAGE_EXTENT_HPP


#include <cstddef>
#include <cstdint>


namespace htracer::benchmarks
{

class image_extent
{
public:
  [[nodiscard]]
  static image_extent
  make(std::uint32_t width, std::uint32_t height);

  [[nodiscard]]
  constexpr std::uint32_t
  width() const noexcept
  {
    return width_;
  }

  [[nodiscard]]
  constexpr std::uint32_t
  height() const noexcept
  {
    return height_;
  }

  [[nodiscard]]
  constexpr std::size_t
  pixel_count() const noexcept
  {
    return static_cast<std::size_t>(width_) * height_;
  }

private:
  constexpr image_extent(std::uint32_t width, std::uint32_t height) noexcept
      : width_{width}
      , height_{height}
  {
  }

  std::uint32_t width_;
  std::uint32_t height_;
};

} // namespace htracer::benchmarks

#endif
