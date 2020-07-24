#include <raytracer/camera.hpp>
#include <raytracer/containertest.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/sampler.hpp>
#include <raytracer/scene.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/vector.hpp>

#include <fstream>
#include <iostream>
#include <variant>


using Float = double;


int
main(int argc, const char* argv[])
{
  std::vector<std::string> args(argv + 1, argv + argc);

  std::string filename = args.size() > 0 ? args[0] : "out.ppm";

  std::vector<hdn::sphere<Float>> spheres;
  std::vector<hdn::light<Float>> lights;

  hdn::v3<Float> camera_pos{{0, 0, 0}};
  hdn::v3<Float> camera_dir{{0, 0, -1}};
  hdn::v3<Float> camera_up{{0, 1, 0}};

  hdn::v3<Float> light_pos{{-5, 5, 0}};
  hdn::v3<Float> light_color{{255, 255, 255}};
  hdn::light<Float> light = {light_pos, light_color, 50};

  lights.push_back(light);

  hdn::material<Float> sphere_material{hdn::v3<Float>{{26, 12, 0}},
                                       hdn::v3<Float>{{128, 50, 0}},
                                       hdn::v3<Float>{{255, 200, 150}},
                                       90};

  hdn::v3<Float> sphere_pos{{0, 0, -5}};
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

  hdn::scene<Float> scene{spheres, lights};
  sample(hdn::ray<Float>{camera_pos, camera_dir}, scene);
  // hdn::camera<Float> cam{camera_pos, camera_dir, camera_up, 480, 272, 60};
  hdn::camera<Float> cam{camera_pos, camera_dir, camera_up, 4096, 2160, 60};

  std::ofstream ofs(filename, std::ios::out | std::ios::binary);
  cam.render(ofs, scene);

  container c;

  c.add(3, 4, 5, 6);
  c.add("hola", "adeu");
  c.add('a', 'b', 'c');

  for (auto it = c.begin(); it != c.end(); ++it)
  {
    std::visit([](auto&& arg) { std::cout << arg.get() << ' '; }, *it);
  }
}
