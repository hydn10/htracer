#ifndef HTRACER_BENCHMARKS_SCENES_MIXED_HPP
#define HTRACER_BENCHMARKS_SCENES_MIXED_HPP


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
make_mixed()
{
  using traits = htracer::float_traits<Float>;

  typename traits::scene scene;

  scene.add_light(
      {.position = {Float{-3}, Float{6}, Float{0}}, .color = {Float{1}, Float{1}, Float{1}}, .intensity = Float{20}});
  scene.add_light(
      {.position = {Float{3}, Float{6}, Float{0}}, .color = {Float{1}, Float{1}, Float{1}}, .intensity = Float{10}});

  auto const floor_material =
      traits::make_solid_material({Float{0.2}, Float{0.2}, Float{0.2}}, Float{0.125}, Float{0}, Float{200}, Float{0.2});
  auto const mirror_material = traits::make_mirror_material(Float{0}, Float{200}, Float{0.92});

  auto const make_sphere_material = [](Float hue)
  {
    auto const color = typename traits::hsl{hue, Float{0.7}, Float{0.5}}.to_srgb().to_linear();
    return traits::make_solid_material(color, Float{0.125}, Float{0.05}, Float{200}, Float{0.4});
  };

  scene.emplace_sphere({{Float{-8.5}, Float{2}, Float{-5}}, Float{2}}, make_sphere_material(Float{310}));
  scene.emplace_sphere({{Float{-1.5}, Float{1}, Float{0}}, Float{1}}, make_sphere_material(Float{42}));
  scene.emplace_sphere({{Float{1.5}, Float{1}, Float{0}}, Float{1}}, make_sphere_material(Float{110}));
  scene.emplace_sphere({{Float{6.8}, Float{1}, Float{-3}}, Float{1}}, make_sphere_material(Float{200}));

  scene.emplace_plane({{Float{0}, Float{0}, Float{0}}, {Float{0}, Float{1}, Float{0}}}, floor_material);
  scene.emplace_plane({{Float{0}, Float{0}, Float{-11}}, {Float{0}, Float{-0.2}, Float{1}}}, mirror_material);

  return {
      .scene = std::move(scene),
      .camera_position = {Float{0}, Float{2.7}, Float{3.1}},
      .camera_view = {Float{0}, Float{-0.4}, Float{-1}},
      .camera_up = {Float{0}, Float{1}, Float{0}},
      .fov = Float{45} * std::numbers::pi_v<Float> / Float{180}};
}

} // namespace htracer::benchmarks::scenes

#endif
