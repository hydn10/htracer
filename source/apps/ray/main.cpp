#include <htracer/geometry/ray.hpp>
#include <htracer/geometry/sphere.hpp>
#include <htracer/raytracing/camera.hpp>
#include <htracer/raytracing/sampler.hpp>
#include <htracer/scene/scene.hpp>
#include <htracer/utils/container.hpp>
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

  std::vector<htracer::geometry::sphere<Float>> spheres;
  std::vector<htracer::scene::light<Float>> lights;

  htracer::v3<Float> camera_pos{{0, 0, 0}};
  htracer::v3<Float> camera_dir{{0, 0, -1}};
  htracer::v3<Float> camera_up{{0, 1, 0}};

  htracer::v3<Float> light_pos{{-5, 5, 0}};
  htracer::v3<Float> light_color{{255, 255, 255}};
  htracer::scene::light<Float> light = {light_pos, light_color, 50};

  lights.push_back(light);

  htracer::scene::material<Float> sphere_material{
      htracer::v3<Float>{{26, 12, 0}},
      htracer::v3<Float>{{128, 50, 0}},
      htracer::v3<Float>{{255, 200, 150}},
      90};

  htracer::v3<Float> sphere_pos{{0, 0, -5}};
  spheres.emplace_back(sphere_pos, 1, sphere_material);

  sphere_pos[0] = 1.5;
  sphere_pos[1] = 2.5;
  spheres.emplace_back(sphere_pos, 1, sphere_material);

  sphere_pos[0] = -1.5;
  sphere_pos[1] = -2.5;
  spheres.emplace_back(sphere_pos, 1, sphere_material);

  sphere_pos[0] = -5;
  sphere_pos[1] = -2.5;
  spheres.emplace_back(sphere_pos, 1, sphere_material);

  htracer::scene::scene<Float> scene{spheres, lights};
  htracer::raytracing::camera<Float> cam{
      camera_pos, camera_dir, camera_up, 3840, 2160, 60};

  std::ofstream ofs(filename, std::ios::out | std::ios::binary);
  cam.render(ofs, scene);

  htracer::utils::container<int, std::string, char> c;

  c.push(3, 4, 5, 6);
  c.push(std::string("hola"), std::string("adeu"));
  c.push('a', 'b', 'c');

  for (auto it: c)
  {
    std::visit([](auto&& arg) { std::cout << arg.get() << ' '; }, it);
  }
}
