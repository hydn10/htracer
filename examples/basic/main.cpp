#include <htracer/htracer.hpp>

#include <numbers>


auto
main() -> int
{
  using ht_f64 = htracer::float_traits<double>;

  ht_f64::scene scene{};

  scene.add_light({.position = {-3., 6., 0.}, .color = {1., 1., 1.}, .intensity = 20});

  auto const floor_material = ht_f64::make_solid_material({0.2, 0.2, 0.2}, 0.125, 0, 200, .2);

  auto const make_sphere_material = [](ht_f64::float_type hue)
  {
    auto const color = ht_f64::hsl{hue, 0.7, 0.5}.to_srgb().to_linear();
    return ht_f64::make_solid_material(color, 0.125, 0.05, 200, .4);
  };

  scene.emplace_sphere({{-1.7, 2.0, -5.0}, 2.0}, make_sphere_material(310));
  scene.emplace_sphere({{-3.5, 1.0, 0.0}, 1.0}, make_sphere_material(42));
  scene.emplace_sphere({{2.5, 1.5, 0.0}, 1.0}, make_sphere_material(200));

  scene.emplace_plane({{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}}, floor_material);

  ht_f64::v3 const cam_pos(0, 2.7, 3.1);
  ht_f64::v3 const cam_view(0, -0.4, -1.);
  ht_f64::v3 const cam_up(0, 1, 0);

  ht_f64::float_type const fov_degrees = 45;
  ht_f64::float_type const fov_rads = fov_degrees * std::numbers::pi / 180;

  ht_f64::camera const camera(cam_pos, cam_view, cam_up, 640, 360, fov_rads);

  // The rendering jobs will be split by column.
  htracer::rendering::batchers::column_batcher const batcher{};

  // The `constant` sensor is the simplest and does not simulate anti-aliasing.
  ht_f64::point_sensor const sensor{};

  // The `pinhole` lens models is the simplest and renders all the image in focus.
  ht_f64::pinhole_lens const lens;

  // `make_renderer` will return the appropriate renderer for the properties of our components.
  // In this case the all our components are deterministic (they have no randomness)
  // and so a deterministic_renderer will be returned.
  auto const renderer = htracer::rendering::make_renderer(batcher, scene, camera, sensor, lens);

  // `unseq` means it will run on a single thread. Use `par_unseq` for multithreading.
  auto const image = renderer.render(htracer::rendering::unseq);

  // Save the image in PPM format with 1 byte per value (3 bytes per pixel).
  htracer::outputs::ppm const ppm{};
  auto constexpr ppm_bpv = htracer::outputs::ppm::bytes_per_value::BPV1;
  ppm.save<ppm_bpv>("basic.ppm", image);
}
