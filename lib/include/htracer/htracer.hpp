#ifndef HTRACER_HTRACER_HPP
#define HTRACER_HTRACER_HPP


// clang-format off
#include <htracer/colors/colors.hpp>

#include <htracer/geometries/geometry.hpp>
#include <htracer/geometries/plane.hpp>
#include <htracer/geometries/ray.hpp>
#include <htracer/geometries/sphere.hpp>

#include <htracer/outputs/ppm.hpp>

#include <htracer/rendering/batchers/column_batcher.hpp>
#include <htracer/rendering/batchers/pixel_batcher.hpp>
#include <htracer/rendering/lenses/pinhole_lens.hpp>
#include <htracer/rendering/lenses/thin_lens.hpp>
#include <htracer/rendering/renderers/deterministic_renderer.hpp>
#include <htracer/rendering/renderers/randomized_renderer.hpp>
#include <htracer/rendering/samplers/deterministic_sampler.hpp>
#include <htracer/rendering/samplers/repeat_sampler.hpp>
#include <htracer/rendering/sensors/point_sensor.hpp>
#include <htracer/rendering/sensors/uniform_sensor.hpp>
#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/concepts.hpp>
#include <htracer/rendering/image.hpp>
#include <htracer/rendering/policies.hpp>
#include <htracer/rendering/rendering.hpp>

#include <htracer/staging/light.hpp>
#include <htracer/staging/material.hpp>
#include <htracer/staging/object.hpp>
#include <htracer/staging/object_base.hpp>
#include <htracer/staging/scene.hpp>
#include <htracer/staging/scene_with.hpp>

#include <htracer/vector.hpp>
// clang-format on

#endif
