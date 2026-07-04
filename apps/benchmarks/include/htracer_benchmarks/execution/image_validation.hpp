#ifndef HTRACER_BENCHMARKS_EXECUTION_IMAGE_VALIDATION_HPP
#define HTRACER_BENCHMARKS_EXECUTION_IMAGE_VALIDATION_HPP


#include <htracer/float_traits.hpp>
#include <htracer_benchmarks/benchmark_result.hpp>
#include <htracer_benchmarks/image_extent.hpp>

#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>


namespace htracer::benchmarks::execution
{

inline constexpr std::uint64_t fnv_offset_basis{14695981039346656037ULL};
inline constexpr std::uint64_t fnv_prime{1099511628211ULL};


inline void
hash_bytes(std::uint64_t &hash, std::span<std::byte const> bytes) noexcept
{
  for (auto const byte : bytes)
  {
    hash ^= std::to_integer<std::uint8_t>(byte);
    hash *= fnv_prime;
  }
}


template<typename T>
void
hash_value(std::uint64_t &hash, T const &value) noexcept
{
  hash_bytes(hash, std::as_bytes(std::span{&value, std::size_t{1}}));
}


template<std::floating_point Float>
[[nodiscard]]
image_checksum
validate_and_hash_image(typename htracer::float_traits<Float>::image const &image, image_extent extent)
{
  if (image.h_res() != extent.width() || image.v_res() != extent.height())
  {
    throw std::runtime_error("render returned unexpected image dimensions");
  }
  if (image.pixels().size() != extent.pixel_count())
  {
    throw std::runtime_error("render returned unexpected pixel count");
  }

  std::uint64_t hash = fnv_offset_basis;
  auto const width = extent.width();
  auto const height = extent.height();
  hash_value(hash, width);
  hash_value(hash, height);

  for (auto const &pixel : image.pixels())
  {
    for (std::size_t channel = 0; channel < 3; ++channel)
    {
      auto const value = pixel[channel];
      if (!std::isfinite(value))
      {
        throw std::runtime_error("render returned a non-finite pixel channel");
      }
      hash_value(hash, value);
    }
  }
  return image_checksum{hash};
}


inline void
combine_hash(std::uint64_t &aggregate, image_checksum checksum) noexcept
{
  auto const value = checksum.value();
  hash_value(aggregate, value);
}

} // namespace htracer::benchmarks::execution

#endif
