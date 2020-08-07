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

  htracer::v3<Float> light_pos{{0., 3., 0.}};
  htracer::v3<Float> light_color{{1., 1., 1.}};

  scene.add_light({light_pos, light_color, 20});

  htracer::scene::material<Float> sphere_material{
      {0.15, 0.15, 0.0}, {0.4, 0.4, 0.0}, {0., 0., 0.}, 200};

  scene.add_object({{{0., 0., 0.}, 1}, sphere_material});

  scene.add_object({{{3.5, 1.5, 0.}, 1.5}, sphere_material});

  scene.add_object({{{-1.5, -2.5, 0.}, 1}, sphere_material});

  scene.add_object({{{-5., -2.5, 0.}, 1}, sphere_material});

  htracer::v3<Float> camera_pos{{0, 0, 8.}};
  htracer::v3<Float> camera_dir{{0, 0, -1.}};
  htracer::v3<Float> camera_up{{0, 1, 0}};

  htracer::raytracing::camera<Float> cam{
      camera_pos, camera_dir, camera_up, 3840, 2160, 50 * std::atan(1) / 45};

  std::ofstream ofs(filename, std::ios::out | std::ios::binary);
  cam.render(ofs, scene);
}
