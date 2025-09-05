#include <htracer/float_traits.hpp>
#include <htracer/outputs/ppm.hpp>
#include <htracer/rendering/batchers/column_batcher.hpp>
#include <htracer/rendering/rendering.hpp>

#include <numbers>
#include <vector>


using ht_f64 = htracer::float_traits<double>;


static auto
build_test_scene()
{
  ht_f64::scene scene;

  scene.add_light({.position = {-3., 6., 0.}, .color = {1., 1., 1.}, .intensity = 20});
  scene.add_light({.position = {3., 6., 0.}, .color = {1., 1., 1.}, .intensity = 10});

  auto const floor_material = ht_f64::make_solid_material({0.2, 0.2, 0.2}, 0.125, 0, 200, .2);
  auto const mirror_material = ht_f64::make_mirror_material(0, 200, 0.92);

  auto const make_sphere_material = [](ht_f64::float_type hue)
  {
    auto const color = ht_f64::hsl{hue, 0.7, 0.5}.to_srgb().to_linear();
    return ht_f64::make_solid_material(color, 0.125, 0.05, 200, .4);
  };

  scene.emplace_sphere({{-8.5, 2.0, -5.0}, 2.0}, make_sphere_material(310));
  scene.emplace_sphere({{-1.5, 1.0, 0.0}, 1.0}, make_sphere_material(42));
  scene.emplace_sphere({{1.5, 1.0, 0.0}, 1.0}, make_sphere_material(110));
  scene.emplace_sphere({{6.8, 1.0, -3.0}, 1.0}, make_sphere_material(200));

  scene.emplace_plane({{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}}, floor_material);
  scene.emplace_plane({{0.0, 0.0, -11.0}, {0.0, -0.2, 1.0}}, mirror_material);

  return scene;
}


int
main(int argc, char const *argv[])
{
  std::vector<std::string_view> const args(argv + 1, argv + argc);

  auto const scene = build_test_scene();

  ht_f64::v3 const camera_pos(0, 2.7, 3.1);
  ht_f64::v3 const camera_view(0, -0.4, -1.);
  ht_f64::v3 const camera_up(0, 1, 0);

  auto const focal_vec = ht_f64::v3(0.0, 1.0, 0.0) - camera_pos;
  // auto const focal_distance = std::sqrt(dot(focal_vec, focal_vec));

  htracer::rendering::batchers::column_batcher batcher;
  // htracer::rendering::batchers::pixel_batcher batcher;

  ht_f64::camera const camera(
      camera_pos, camera_view, camera_up, 1024, 576, 45 * std::numbers::pi / 180);

  // ht_f64::point_sensor const point_sensor;
  ht_f64::uniform_sensor sensor{};

  ht_f64::pinhole_lens const lens;
  // ht_f64::thin_lens lens(0.2, 3);

  auto const renderer = htracer::rendering::make_renderer(batcher, scene, camera, sensor, lens);
  auto const image = renderer.render(htracer::rendering::par_unseq, 20);

  auto const filename = args.size() > 0 ? args[0] : "out.ppm";

  htracer::outputs::ppm const ppm{};
  auto constexpr ppmbpv = htracer::outputs::ppm::bytes_per_value::BPV1;
  ppm.save<ppmbpv>(filename, image);
}
