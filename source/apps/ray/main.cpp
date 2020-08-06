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

  htracer::v3<Float> light_pos{{-5, 5, 0}};
  htracer::v3<Float> light_color{{255, 255, 255}};

  scene.add_light({light_pos, light_color, 50});

  htracer::scene::material<Float> sphere_material{
      htracer::v3<Float>{{26, 12, 0}},
      htracer::v3<Float>{{128, 50, 0}},
      htracer::v3<Float>{{255, 200, 150}},
      90};

  htracer::v3<Float> sphere_pos{{0, 0, -5}};
  scene.add_object({{sphere_pos, 1}, sphere_material});

  sphere_pos[0] = 1.5;
  sphere_pos[1] = 2.5;
  scene.add_object({{sphere_pos, 1}, sphere_material});

  sphere_pos[0] = -1.5;
  sphere_pos[1] = -2.5;
  scene.add_object({{sphere_pos, 1}, sphere_material});

  sphere_pos[0] = -5;
  sphere_pos[1] = -2.5;
  scene.add_object({{sphere_pos, 1}, sphere_material});

  htracer::v3<Float> camera_pos{{0, 0, 0}};
  htracer::v3<Float> camera_dir{{0, 0, -1}};
  htracer::v3<Float> camera_up{{0, 1, 0}};

  htracer::raytracing::camera<Float> cam{
      camera_pos, camera_dir, camera_up, 3840, 2160, 60};

  std::ofstream ofs(filename, std::ios::out | std::ios::binary);
  cam.render(ofs, scene);
}
