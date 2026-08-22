#ifndef HTRACER_BENCHMARKS_SCENES_TRAVERSAL_HPP
#define HTRACER_BENCHMARKS_SCENES_TRAVERSAL_HPP


#include <htracer/htracer.hpp>
#include <htracer_benchmarks/scene_spec.hpp>
#include <htracer_benchmarks/scenes/scene_setup.hpp>

#include <concepts>
#include <cstdint>
#include <numbers>
#include <utility>


namespace htracer::benchmarks::scenes
{

template<std::floating_point Float>
[[nodiscard]]
scene_setup<Float>
make_traversal(geometry_count count);


template<std::floating_point Float>
[[nodiscard]]
scene_setup<Float>
make_traversal(geometry_count count)
{
  using traits = htracer::float_traits<Float>;

  typename traits::scene scene;
  auto const visible_material =
      traits::make_solid_material({Float{0.8}, Float{0.35}, Float{0.15}}, Float{0.8}, Float{0}, Float{1}, Float{0});
  auto const hidden_material =
      traits::make_solid_material({Float{0.2}, Float{0.2}, Float{0.2}}, Float{0}, Float{0}, Float{1}, Float{0});

  scene.emplace_sphere({{Float{0}, Float{0}, Float{-3}}, Float{1}}, visible_material);

  for (std::uint32_t index = 1; index < count.value(); ++index)
  {
    auto const column = index % std::uint32_t{1024};
    auto const row = index / std::uint32_t{1024};
    auto const x = Float{1000} + Float{3} * static_cast<Float>(column);
    auto const y = Float{1000} + Float{3} * static_cast<Float>(row);
    auto const z = Float{-10} - static_cast<Float>(row);
    scene.emplace_sphere({{x, y, z}, Float{1}}, hidden_material);
  }

  return {
      .scene = std::move(scene),
      .camera_position = {Float{0}, Float{0}, Float{0}},
      .camera_view = {Float{0}, Float{0}, Float{-1}},
      .camera_up = {Float{0}, Float{1}, Float{0}},
      .fov = Float{45} * std::numbers::pi_v<Float> / Float{180}};
}

} // namespace htracer::benchmarks::scenes

#endif
