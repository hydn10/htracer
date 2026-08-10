#ifndef HTRACER_BENCHMARKS_SCENES_MAKE_SCENE_HPP
#define HTRACER_BENCHMARKS_SCENES_MAKE_SCENE_HPP


#include <htracer_benchmarks/scene_spec.hpp>
#include <htracer_benchmarks/scenes/mixed.hpp>
#include <htracer_benchmarks/scenes/rng_probe.hpp>
#include <htracer_benchmarks/scenes/scene_setup.hpp>
#include <htracer_benchmarks/scenes/traversal.hpp>

#include <concepts>
#include <variant>


namespace htracer::benchmarks::scenes
{

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
          return make_mixed<Float>();
        }
        else if constexpr (std::same_as<Scene, traversal_scene>)
        {
          return make_traversal<Float>(scene.count);
        }
        else
        {
          static_assert(std::same_as<Scene, rng_probe_scene>);
          return make_rng_probe<Float>();
        }
      },
      specification);
}

} // namespace htracer::benchmarks::scenes

#endif
