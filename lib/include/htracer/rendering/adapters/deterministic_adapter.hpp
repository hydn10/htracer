#ifndef HTRACER_RENDERING_ADAPTERS_DETERMINISTIC_ADAPTER_HPP
#define HTRACER_RENDERING_ADAPTERS_DETERMINISTIC_ADAPTER_HPP


#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>


namespace htracer::rendering::adapters::detail_
{

template<typename Float, deterministic_sensor<Float> Sensor, deterministic_lens<Float> Lens>
class deterministic_adapter
{
  Sensor const &sensor_;
  Lens const &lens_;

public:
  constexpr deterministic_adapter(Sensor const &sensor, Lens const &lens) noexcept;

  constexpr auto
  get_coords(uint32_t v_idx, uint32_t h_idx) const;

  constexpr auto
  get_ray(Float dv, Float dh, camera<Float> const &camera) const;
};


template<typename Float, deterministic_sensor<Float> Sensor, deterministic_lens<Float> Lens>
constexpr deterministic_adapter<Float, Sensor, Lens>::deterministic_adapter(Sensor const &sensor, Lens const &lens) noexcept
    : sensor_{sensor}
    , lens_{lens}
{
}


template<typename Float, deterministic_sensor<Float> Sensor, deterministic_lens<Float> Lens>
constexpr auto
deterministic_adapter<Float, Sensor, Lens>::get_coords(uint32_t v_idx, uint32_t h_idx) const
{
  return sensor_.get_coords(v_idx, h_idx);
}


template<typename Float, deterministic_sensor<Float> Sensor, deterministic_lens<Float> Lens>
constexpr auto
deterministic_adapter<Float, Sensor, Lens>::get_ray(Float dv, Float dh, camera<Float> const &camera) const
{
  return lens_.get_ray(dv, dh, camera.position(), camera.view(), camera.up(), camera.right());
}

} // namespace htracer::rendering::adapters::detail_

#endif
