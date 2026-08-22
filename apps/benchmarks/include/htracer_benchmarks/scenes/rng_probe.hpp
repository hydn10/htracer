#ifndef HTRACER_BENCHMARKS_SCENES_RNG_PROBE_HPP
#define HTRACER_BENCHMARKS_SCENES_RNG_PROBE_HPP


#include <htracer/htracer.hpp>
#include <htracer_benchmarks/scenes/scene_setup.hpp>

#include <concepts>
#include <numbers>
#include <utility>


namespace htracer::benchmarks::scenes
{

template<std::floating_point Float>
[[nodiscard]]
scene_setup<Float>
make_rng_probe();


template<std::floating_point Float>
[[nodiscard]]
scene_setup<Float>
make_rng_probe()
{
  using traits = htracer::float_traits<Float>;

  typename traits::scene scene;
  auto const material =
      traits::make_solid_material({Float{0.9}, Float{0.4}, Float{0.1}}, Float{1}, Float{0}, Float{1}, Float{0});

  scene.emplace_sphere({{Float{0.15}, Float{0}, Float{-2.5}}, Float{1.15}}, material);

  return {
      .scene = std::move(scene),
      .camera_position = {Float{0}, Float{0}, Float{0}},
      .camera_view = {Float{0}, Float{0}, Float{-1}},
      .camera_up = {Float{0}, Float{1}, Float{0}},
      .fov = Float{45} * std::numbers::pi_v<Float> / Float{180}};
}

} // namespace htracer::benchmarks::scenes

#endif
