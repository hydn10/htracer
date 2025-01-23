#include <htracer/geometries/plane.hpp>
#include <htracer/geometries/sphere.hpp>
#include <htracer/outputs/ppm.hpp>
#include <htracer/rendering/batchers/column_batcher.hpp>
#include <htracer/rendering/batchers/pixel_batcher.hpp>
#include <htracer/rendering/camera.hpp>
#include <htracer/rendering/lenses/pinhole_lens.hpp>
#include <htracer/rendering/lenses/thin_lens.hpp>
#include <htracer/rendering/policies.hpp>
#include <htracer/rendering/rendering.hpp>
#include <htracer/rendering/sensors/point_sensor.hpp>
#include <htracer/rendering/sensors/uniform_sensor.hpp>
#include <htracer/staging/scene_with.hpp>
#include <htracer/vector.hpp>

#include <numbers>
#include <vector>


using Float = double;


static constexpr htracer::staging::material<Float>
get_sphere_material(Float hue)
{
  using htracer::colors::hsl;

  auto const color = hsl<Float>{hue, 0.7, 0.5}.to_srgb().to_linear();
  return htracer::staging::make_solid<Float>(color, 0.125, 0.05, 200, .4);
}


template<typename Float>
using sphere_t = htracer::geometries::sphere<Float>;
template<typename Float>
using plane_t = htracer::geometries::plane<Float>;


static auto
build_test_scene()
{
  htracer::staging::scene_with<Float, sphere_t, plane_t> scene;

  scene.add_light({{-3., 6., 0.}, {1., 1., 1.}, 20});
  scene.add_light({{3., 6., 0.}, {1., 1., 1.}, 10});

  auto const floor_material = htracer::staging::make_solid<Float>({0.2, 0.2, 0.2}, 0.125, 0, 200, .2);
  auto const mirror_material = htracer::staging::make_mirror<Float>(0, 200, 0.92);

  scene.emplace_object<sphere_t>({{-8.5, 2.0, -5.0}, 2.0}, get_sphere_material(310));
  scene.emplace_object<sphere_t>({{-1.5, 1.0, 0.0}, 1.0}, get_sphere_material(42));
  scene.emplace_object<sphere_t>({{1.5, 1.0, 0.0}, 1.0}, get_sphere_material(110));
  scene.emplace_object<sphere_t>({{6.8, 1.0, -3.0}, 1.0}, get_sphere_material(200));

  scene.emplace_object<plane_t>({{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}}, floor_material);
  scene.emplace_object<plane_t>({{0.0, 0.0, -11.0}, {0.0, -0.2, 1.0}}, mirror_material);

  return scene;
}


int
main(int argc, char const *argv[])
{
  std::vector<std::string_view> const args(argv + 1, argv + argc);

  auto const scene = build_test_scene();

  htracer::v3<Float> const camera_pos(0, 2.7, 3.1);
  htracer::v3<Float> const camera_view(0, -0.4, -1.);
  htracer::v3<Float> const camera_up(0, 1, 0);

  auto const focal_vec = htracer::v3<Float>(0.0, 1.0, 0.0) - camera_pos;
  // auto const focal_distance = std::sqrt(dot(focal_vec, focal_vec));

  // htracer::v3<Float> const camera_pos(8, 4, 0.4);
  // htracer::v3<Float> const camera_dir = htracer::v3<Float>(1.5, 1.0, -3.0) - camera_pos;
  // htracer::v3<Float> const camera_up(0, 1, 0);

  htracer::rendering::batchers::column_batcher batcher;
  // htracer::rendering::batchers::pixel_batcher batcher;

  htracer::rendering::camera const camera(
      camera_pos, camera_view, camera_up, 1024, 576, 45 * std::numbers::pi_v<Float> / 180);

  // htracer::rendering::sensors::point_sensor<Float> const sensor;
  htracer::rendering::sensors::uniform_sensor<Float> sensor;

  htracer::rendering::lenses::pinhole_lens<Float> const lens;
  // htracer::rendering::lenses::thin_lens<Float> lens(0.2, 3);

  auto const renderer = htracer::rendering::make_renderer(batcher, scene, camera, sensor, lens);
  auto const image = renderer.render(htracer::rendering::unseq, 100);

  auto const filename = args.size() > 0 ? args[0] : "out.ppm";

  htracer::outputs::ppm const ppm;
  auto constexpr ppmbpv = htracer::outputs::ppm::bytes_per_value::BPV1;
  ppm.save<ppmbpv>(filename, image);
}
