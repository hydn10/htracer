#ifndef HTRACER_BENCHMARKS_SCENES_SCENE_SETUP_HPP
#define HTRACER_BENCHMARKS_SCENES_SCENE_SETUP_HPP


#include <htracer/float_traits.hpp>

#include <concepts>


namespace htracer::benchmarks::scenes
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

} // namespace htracer::benchmarks::scenes

#endif
