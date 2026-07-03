#ifndef HTRACER_BENCHMARKS_SCENES_HPP
#define HTRACER_BENCHMARKS_SCENES_HPP


#include <htracer/htracer.hpp>
#include <htracer_benchmarks/model.hpp>

#include <concepts>
#include <numbers>
#include <utility>
#include <variant>


namespace htracer::benchmarks
{

template<std::floating_point Float>
struct scene_setup
{
  using traits = htracer::float_traits<Float>;

  typename traits::scene scene;
  typename traits::v3 camera_position;
  typename traits::v3 camera_view;
  typename traits::v3 camera_up;
  Float fov;
};


namespace detail_
{

template<std::floating_point Float>
[[nodiscard]]
scene_setup<Float>
make_mixed_scene()
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


template<std::floating_point Float>
[[nodiscard]]
scene_setup<Float>
make_traversal_scene(geometry_count count)
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


template<std::floating_point Float>
[[nodiscard]]
scene_setup<Float>
make_rng_probe_scene()
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

} // namespace detail_


template<std::floating_point Float>
[[nodiscard]]
scene_setup<Float>
make_scene(scene_spec const &specification)
{
  return std::visit(
      []<typename Scene>(Scene const &scene) -> scene_setup<Float>
  {
    if constexpr (std::same_as<Scene, mixed_scene>)
    {
      return detail_::make_mixed_scene<Float>();
    }
    else if constexpr (std::same_as<Scene, traversal_scene>)
    {
      return detail_::make_traversal_scene<Float>(scene.count);
    }
    else
    {
      static_assert(std::same_as<Scene, rng_probe_scene>);
      return detail_::make_rng_probe_scene<Float>();
    }
  },
      specification);
}

} // namespace htracer::benchmarks

#endif
