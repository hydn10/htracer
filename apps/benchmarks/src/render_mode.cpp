#include <htracer_benchmarks/render_mode.hpp>

#include <htracer/rendering/random_seed.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>

#include <optional>
#include <stdexcept>


namespace htracer::benchmarks
{

randomized_render
randomized_render::make(
    htracer::rendering::samples_per_pixel samples, std::optional<htracer::rendering::random_seed> seed)
{
  if (samples.value == 0)
  {
    throw std::invalid_argument("samples per pixel must be greater than zero");
  }
  return randomized_render{samples, seed};
}

} // namespace htracer::benchmarks
