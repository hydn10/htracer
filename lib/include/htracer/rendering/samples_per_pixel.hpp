#ifndef HTRACER_RENDERING_SAMPLES_PER_PIXEL_HPP
#define HTRACER_RENDERING_SAMPLES_PER_PIXEL_HPP


#include <cstdint>
#include <expected>
#include <stdexcept>
#include <string_view>


namespace htracer::rendering
{

class samples_per_pixel
{
  struct unchecked
  {
  };

  explicit constexpr samples_per_pixel([[maybe_unused]] unchecked unchecked, std::uint32_t value) noexcept;

  [[nodiscard]]
  static constexpr bool is_valid(std::uint32_t value) noexcept;

  std::uint32_t value_;

public:
  [[nodiscard]]
  static constexpr std::expected<samples_per_pixel, std::string_view>
  try_make(std::uint32_t value) noexcept;

  explicit constexpr samples_per_pixel(std::uint32_t value);

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept;
};


constexpr samples_per_pixel::samples_per_pixel(
    [[maybe_unused]] unchecked unchecked,
    std::uint32_t value) noexcept
    : value_{value}
{
}


constexpr bool
samples_per_pixel::is_valid(std::uint32_t value) noexcept
{
  return value != 0;
}


constexpr samples_per_pixel::samples_per_pixel(std::uint32_t value)
    : value_{value}
{
  if (!is_valid(value))
  {
    throw std::invalid_argument("samples per pixel must be greater than zero");
  }
}


constexpr std::uint32_t
samples_per_pixel::value() const noexcept
{
  return value_;
}


constexpr std::expected<samples_per_pixel, std::string_view>
samples_per_pixel::try_make(std::uint32_t value) noexcept
{
  if (!is_valid(value))
  {
    return std::unexpected{"samples per pixel must be greater than zero"};
  }
  return samples_per_pixel{unchecked{}, value};
}

} // namespace htracer::rendering

#endif
