#include <htracer/output/ppm.hpp>
#include <htracer/raytracing/camera.hpp>
#include <htracer/raytracing/sampler.hpp>
#include <htracer/scene/scene.hpp>
#include <htracer/vector.hpp>

#include <fstream>
#include <iostream>
#include <variant>


using Float = double;


htracer::scene::material<Float>
get_sphere_material(Float hue)
{
  using htracer::colors::hsl;

  const auto color = hsl<Float>{hue, 0.7, 0.5}.to_srgb().to_linear();
  return htracer::scene::make_solid<Float>(color, 0.125, 0.05, 200, .4);
}


int
main(int argc, const char* argv[])
{
  std::vector<std::string> args(argv + 1, argv + argc);

  std::string filename = args.size() > 0 ? args[0] : "out.ppm";

  htracer::scene::scene<Float> scene;

  scene.add_light({{-3., 6., 0.}, {1., 1., 1.}, 20});
  scene.add_light({{3., 6., 0.}, {1., 1., 1.}, 10});

  const auto floor_material =
      htracer::scene::make_solid<Float>({0.2, 0.2, 0.2}, 0.125, 0, 200, .2);
  const auto mirror_material = htracer::scene::make_mirror<Float>(0, 200, 0.92);

  using sphere = htracer::geometry::sphere<Float>;
  using plane = htracer::geometry::plane<Float>;

  // clang-format off
  scene.add_object({sphere{{-8.5,  2.0, -5.0}, 2.0}, get_sphere_material(310)});
  scene.add_object({sphere{{-1.5,  1.0,  0.0}, 1.0}, get_sphere_material(42)});
  scene.add_object({sphere{{ 1.5,  1.0,  0.0}, 1.0}, get_sphere_material(110)});
  scene.add_object({sphere{{ 6.8,  1.0, -3.0}, 1.0}, get_sphere_material(200)});
  // clang-format on

  scene.add_object({plane{{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}}, floor_material});
  scene.add_object(
      {plane{{0.0, 0.0, -11.0}, {0.0, -0.2, 1.0}}, mirror_material});

  htracer::v3<Float> camera_pos{{0, 2.7, 3.1}};
  htracer::v3<Float> camera_dir{{0, -0.4, -1.}};
  htracer::v3<Float> camera_up{{0, 1, 0}};

  htracer::raytracing::camera<Float> cam{
      camera_pos,
      camera_dir,
      camera_up,
      3840,
      2160,
      60 * std::atan(Float{1}) / 45};

  std::ofstream ofs(filename, std::ios::out | std::ios::binary);
  htracer::output::ppm ppm(ofs);

  ppm << cam.render(scene);
}
