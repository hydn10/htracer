#include <raytracer/vector.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/sampler.hpp>
#include <raytracer/camera.hpp>

#include <fstream>


using Float = double;


int
main()
{
  hdn::v3<Float> camera_pos{{0, 0, 0}};
  hdn::v3<Float> camera_dir{{0, 0, -1}};
  hdn::v3<Float> camera_up{{0, 1, 0}};

  hdn::v3<Float> sphere_pos{{0, 0, -5}};

  auto s = hdn::sphere<Float>{sphere_pos, 1};

  std::vector<hdn::sphere<Float>> spheres{s};

  sphere_pos[0] = 1.5;
  sphere_pos[1] = 2.5;
  spheres.emplace_back(sphere_pos, 1);

  sphere_pos[0] = -1.5;
  sphere_pos[1] = -2.5;
  spheres.emplace_back(sphere_pos, 1);

  sphere_pos[0] = -5;
  sphere_pos[1] = -2.5;
  spheres.emplace_back(sphere_pos, 1);

  hdn::camera<Float> cam{camera_pos, camera_dir, camera_up, 480, 272, 60};

  std::ofstream ofs("./test.ppm", std::ios::out | std::ios::binary); 
  cam.render(ofs, spheres);
}