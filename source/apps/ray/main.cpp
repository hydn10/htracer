#include <htracer/output/ppm.hpp>
#include <htracer/raytracing/camera.hpp>
#include <htracer/raytracing/sampler.hpp>
#include <htracer/scene/scene.hpp>
#include <htracer/vector.hpp>

#include <fstream>
#include <iostream>
#include <variant>


using Float = double;


int
main(int argc, const char* argv[])
{
  std::vector<std::string> args(argv + 1, argv + argc);

  std::string filename = args.size() > 0 ? args[0] : "out.ppm";

  htracer::scene::scene<Float> scene;

  scene.add_light({{0., 4., 0.}, {1., 1., 1.}, 20});
  scene.add_light({{3., 7., -2.}, {1., 1., 1.}, 10});

  htracer::scene::material<Float> sphere_material{
      {0.025, 0.025, 0.0}, {0.2, 0.2, 0.0}, 0.05, 200, .6};
  htracer::scene::material<Float> plane_material{
      {0.20, 0.02, 0.05}, {0.7, 0.05, 0.1}, 0, 200, .0};

  using sphere = htracer::geometry::sphere<Float>;
  using plane = htracer::geometry::plane<Float>;

  // clang-format off
  scene.add_object({sphere{{ 0.0,  1.0,  0.0}, 1.0}, sphere_material});
  scene.add_object({sphere{{-2.5,  1.0,  0.0}, 1.0}, sphere_material});
  scene.add_object({sphere{{ 3.5,  1.5,  0.0}, 1.5}, sphere_material});
  scene.add_object({sphere{{-5.0,  3.5 , 0.0}, 1.0}, sphere_material});

  scene.add_object({plane{{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}}, plane_material});
  // clang-format on

  htracer::v3<Float> camera_pos{{0, 2.3, 5.}};
  htracer::v3<Float> camera_dir{{0, -0.2, -1.}};
  htracer::v3<Float> camera_up{{0, 1, 0}};

  htracer::raytracing::camera<Float> cam{
      camera_pos, camera_dir, camera_up, 3840, 2160, 60 * std::atan(1) / 45};

  std::ofstream ofs(filename, std::ios::out | std::ios::binary);
  htracer::output::ppm ppm(ofs);

  ppm << cam.render(scene);
}
