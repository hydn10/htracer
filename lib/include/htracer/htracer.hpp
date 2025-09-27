#ifndef HTRACER_HTRACER_HPP
#define HTRACER_HTRACER_HPP


#include <htracer/colors/colors.hpp> // IWYU pragma: export

#include <htracer/geometries/geometry.hpp> // IWYU pragma: export
#include <htracer/geometries/plane.hpp>    // IWYU pragma: export
#include <htracer/geometries/ray.hpp>      // IWYU pragma: export
#include <htracer/geometries/sphere.hpp>   // IWYU pragma: export

#include <htracer/outputs/ppm.hpp> // IWYU pragma: export

#include <htracer/rendering/batchers/column_batcher.hpp>          // IWYU pragma: export
#include <htracer/rendering/batchers/pixel_batcher.hpp>           // IWYU pragma: export
#include <htracer/rendering/camera.hpp>                           // IWYU pragma: export
#include <htracer/rendering/concepts.hpp>                         // IWYU pragma: export
#include <htracer/rendering/image.hpp>                            // IWYU pragma: export
#include <htracer/rendering/lenses/pinhole_lens.hpp>              // IWYU pragma: export
#include <htracer/rendering/lenses/thin_lens.hpp>                 // IWYU pragma: export
#include <htracer/rendering/policies.hpp>                         // IWYU pragma: export
#include <htracer/rendering/renderers/deterministic_renderer.hpp> // IWYU pragma: export
#include <htracer/rendering/renderers/randomized_renderer.hpp>    // IWYU pragma: export
#include <htracer/rendering/rendering.hpp>                        // IWYU pragma: export
#include <htracer/rendering/samplers/deterministic_sampler.hpp>   // IWYU pragma: export
#include <htracer/rendering/samplers/repeat_sampler.hpp>          // IWYU pragma: export
#include <htracer/rendering/sensors/point_sensor.hpp>             // IWYU pragma: export
#include <htracer/rendering/sensors/uniform_sensor.hpp>           // IWYU pragma: export

#include <htracer/staging/light.hpp>       // IWYU pragma: export
#include <htracer/staging/material.hpp>    // IWYU pragma: export
#include <htracer/staging/object.hpp>      // IWYU pragma: export
#include <htracer/staging/object_base.hpp> // IWYU pragma: export
#include <htracer/staging/scene.hpp>       // IWYU pragma: export
#include <htracer/staging/scene_with.hpp>  // IWYU pragma: export

#include <htracer/float_traits.hpp> // IWYU pragma: export
#include <htracer/vector.hpp>       // IWYU pragma: export

#endif
