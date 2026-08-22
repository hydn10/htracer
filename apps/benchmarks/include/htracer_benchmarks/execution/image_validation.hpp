#ifndef HTRACER_BENCHMARKS_EXECUTION_IMAGE_VALIDATION_HPP
#define HTRACER_BENCHMARKS_EXECUTION_IMAGE_VALIDATION_HPP


#include <htracer/float_traits.hpp>
#include <htracer_benchmarks/benchmark_result.hpp>
#include <htracer_benchmarks/execution/checksum_accumulator.hpp>
#include <htracer_benchmarks/image_extent.hpp>

#include <cmath>
#include <concepts>
#include <cstddef>
#include <stdexcept>


namespace htracer::benchmarks::execution
{

template<std::floating_point Float>
[[nodiscard]]
image_checksum
validate_and_hash_image(typename htracer::float_traits<Float>::image const &image, image_extent extent);


template<std::floating_point Float>
[[nodiscard]]
image_checksum
validate_and_hash_image(typename htracer::float_traits<Float>::image const &image, image_extent extent)
{
  if (image.h_res() != extent.width().value() || image.v_res() != extent.height().value())
  {
    throw std::runtime_error("render returned unexpected image dimensions");
  }
  if (image.pixels().size() != extent.pixel_count())
  {
    throw std::runtime_error("render returned unexpected pixel count");
  }

  checksum_accumulator hash;
  auto const width = extent.width().value();
  auto const height = extent.height().value();
  hash.append(width);
  hash.append(height);

  for (auto const &pixel : image.pixels())
  {
    for (auto const value : pixel)
    {
      if (!std::isfinite(value))
      {
        throw std::runtime_error("render returned a non-finite pixel channel");
      }
      hash.append(value);
    }
  }
  return image_checksum{hash.value()};
}

} // namespace htracer::benchmarks::execution

#endif
