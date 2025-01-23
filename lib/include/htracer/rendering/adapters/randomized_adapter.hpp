#ifndef HTRACER_RENDERING_ADAPTERS_RANDOMIZED_ADAPTER_HPP
#define HTRACER_RENDERING_ADAPTERS_RANDOMIZED_ADAPTER_HPP


#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>


namespace htracer::rendering::adapters::detail_
{

template<typename Float, sensor<Float> Sensor, lens<Float> Lens, std::uniform_random_bit_generator Generator>
class randomized_adapter
{
  Sensor const &sensor_;
  Lens const &lens_;
  Generator &generator_;

public:
  randomized_adapter(Sensor const &sensor, Lens const &lens, Generator &generator) noexcept;

  auto
  get_coords(uint32_t v_idx, uint32_t h_idx) const;

  auto
  get_ray(Float dv, Float dh, camera<Float> const &camera) const;
};


template<typename Float, sensor<Float> Sensor, lens<Float> Lens, std::uniform_random_bit_generator Generator>
randomized_adapter<Float, Sensor, Lens, Generator>::randomized_adapter(
    Sensor const &sensor, Lens const &lens, Generator &generator) noexcept
    : sensor_{sensor}
    , lens_{lens}
    , generator_{generator}
{
}


template<typename Float, sensor<Float> Sensor, lens<Float> Lens, std::uniform_random_bit_generator Generator>
auto
randomized_adapter<Float, Sensor, Lens, Generator>::get_coords(uint32_t v_idx, uint32_t h_idx) const
{
  if constexpr (deterministic_sensor<Sensor, Float>)
  {
    return sensor_.get_coords(v_idx, h_idx);
  }
  else
  {
    return sensor_.get_coords(v_idx, h_idx, generator_);
  }
}


template<typename Float, sensor<Float> Sensor, lens<Float> Lens, std::uniform_random_bit_generator Generator>
auto
randomized_adapter<Float, Sensor, Lens, Generator>::get_ray(Float dv, Float dh, camera<Float> const &camera) const
{
  if constexpr (deterministic_lens<Lens, Float>)
  {
    return lens_.get_ray(dv, dh, camera.position(), camera.view(), camera.up(), camera.right());
  }
  else
  {
    return lens_.get_ray(dv, dh, camera.position(), camera.view(), camera.up(), camera.right(), generator_);
  }
}

} // namespace htracer::rendering::adapters::detail_

#endif
