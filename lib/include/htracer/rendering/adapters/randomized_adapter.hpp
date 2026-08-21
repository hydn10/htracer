#ifndef HTRACER_RENDERING_ADAPTERS_RANDOMIZED_ADAPTER_HPP
#define HTRACER_RENDERING_ADAPTERS_RANDOMIZED_ADAPTER_HPP


#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>

#include <cstdint>
#include <functional>


namespace htracer::rendering::adapters::detail_
{

template<
    typename Float,
    std::uniform_random_bit_generator Generator,
    sensor<Float, Generator> Sensor,
    lens<Float, Generator> Lens>
class randomized_adapter
{
  std::reference_wrapper<Sensor const> sensor_;
  std::reference_wrapper<Lens const> lens_;
  std::reference_wrapper<Generator> generator_;

public:
  randomized_adapter(
      std::reference_wrapper<Sensor const> sensor,
      std::reference_wrapper<Lens const> lens,
      Generator &generator) noexcept;

  [[nodiscard]]
  auto
  get_coords(uint32_t v_idx, uint32_t h_idx) const;

  [[nodiscard]]
  auto
  get_ray(Float dv, Float dh, camera<Float> const &camera) const;
};


template<
    typename Float,
    std::uniform_random_bit_generator Generator,
    sensor<Float, Generator> Sensor,
    lens<Float, Generator> Lens>
randomized_adapter<Float, Generator, Sensor, Lens>::randomized_adapter(
    std::reference_wrapper<Sensor const> sensor,
    std::reference_wrapper<Lens const> lens,
    Generator &generator) noexcept
    : sensor_{sensor}
    , lens_{lens}
    , generator_{generator}
{
}


template<
    typename Float,
    std::uniform_random_bit_generator Generator,
    sensor<Float, Generator> Sensor,
    lens<Float, Generator> Lens>
auto
randomized_adapter<Float, Generator, Sensor, Lens>::get_coords(uint32_t v_idx, uint32_t h_idx) const
{
  if constexpr (deterministic_sensor<Sensor, Float>)
  {
    return sensor_.get().get_coords(v_idx, h_idx);
  }
  else
  {
    return sensor_.get().get_coords(v_idx, h_idx, generator_.get());
  }
}


template<
    typename Float,
    std::uniform_random_bit_generator Generator,
    sensor<Float, Generator> Sensor,
    lens<Float, Generator> Lens>
auto
randomized_adapter<Float, Generator, Sensor, Lens>::get_ray(Float dv, Float dh, camera<Float> const &camera) const
{
  if constexpr (deterministic_lens<Lens, Float>)
  {
    return lens_.get().get_ray(dv, dh, camera.position(), camera.view(), camera.up(), camera.right());
  }
  else
  {
    return lens_.get().get_ray(dv, dh, camera.position(), camera.view(), camera.up(), camera.right(), generator_.get());
  }
}

} // namespace htracer::rendering::adapters::detail_

#endif
