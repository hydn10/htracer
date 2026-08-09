#ifndef HTRACER_BENCHMARKS_EXECUTION_RENDER_PIPELINE_HPP
#define HTRACER_BENCHMARKS_EXECUTION_RENDER_PIPELINE_HPP


#include <htracer/rendering/batchers/column_batcher.hpp>
#include <htracer/rendering/lenses/pinhole_lens.hpp>
#include <htracer/rendering/sensors/point_sensor.hpp>
#include <htracer/rendering/sensors/uniform_sensor.hpp>
#include <htracer_benchmarks/render_mode.hpp>

#include <concepts>
#include <string_view>


namespace htracer::benchmarks::execution
{

template<typename Mode>
struct render_pipeline;


template<>
struct render_pipeline<deterministic_render>
{
  using batcher = htracer::rendering::batchers::column_batcher;

  template<std::floating_point Float>
  using sensor = htracer::rendering::sensors::point_sensor<Float>;

  template<std::floating_point Float>
  using lens = htracer::rendering::lenses::pinhole_lens<Float>;

  static constexpr std::string_view sensor_name{"point"};
  static constexpr std::string_view batcher_name{"column"};
  static constexpr std::string_view lens_name{"pinhole"};
};


template<>
struct render_pipeline<randomized_render>
{
  using batcher = htracer::rendering::batchers::column_batcher;

  template<std::floating_point Float>
  using sensor = htracer::rendering::sensors::uniform_sensor<Float>;

  template<std::floating_point Float>
  using lens = htracer::rendering::lenses::pinhole_lens<Float>;

  static constexpr std::string_view sensor_name{"uniform"};
  static constexpr std::string_view batcher_name{"column"};
  static constexpr std::string_view lens_name{"pinhole"};
};

} // namespace htracer::benchmarks::execution

#endif
