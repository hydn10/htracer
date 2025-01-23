#include <htracer/htracer.hpp>


int
main()
{
  htracer::staging::scene<double> scene;

  scene.add_light({{-3., 6., 0.}, {1., 1., 1.}, 20});

  auto const floor_material = htracer::staging::make_solid<double>({0.2, 0.2, 0.2}, 0.125, 0, 200, .2);

  auto const make_sphere_material = [](double hue)
  {
    auto const color = htracer::colors::hsl<double>{hue, 0.7, 0.5}.to_srgb().to_linear();
    return htracer::staging::make_solid<double>(color, 0.125, 0.05, 200, .4);
  };

  scene.emplace_sphere({{-1.7, 2.0, -5.0}, 2.0}, make_sphere_material(310));
  scene.emplace_sphere({{-3.5, 1.0, 0.0}, 1.0}, make_sphere_material(42));
  scene.emplace_sphere({{2.5, 1.5, 0.0}, 1.0}, make_sphere_material(200));

  scene.emplace_plane({{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}}, floor_material);

  htracer::v3<double> const cam_pos(0, 2.7, 3.1);
  htracer::v3<double> const cam_view(0, -0.4, -1.);
  htracer::v3<double> const cam_up(0, 1, 0);

  double const fov_degrees = 45;
  double const fov_rads = fov_degrees * std::numbers::pi / 180;

  htracer::rendering::camera const camera(cam_pos, cam_view, cam_up, 640, 360, fov_rads);

  // The rendering jobs will be split by column.
  htracer::rendering::batchers::column_batcher batcher;

  // The `constant` sensor is the simplest and does not simulate anti-aliasing.
  htracer::rendering::sensors::point_sensor<double> const sensor;

  // The `pinhole` lens models is the simplest and renders all the image in focus.
  htracer::rendering::lenses::pinhole_lens<double> const lens;

  // `make_renderer` will return the appropiate renderer for the properties of our components.
  // In this case the all our components are deterministic (they have no randomness)
  // and so a deterministic_renderer will be returned.
  auto const renderer = htracer::rendering::make_renderer(batcher, scene, camera, sensor, lens);

  // `unseq` means it will run on a single thread. Use `par_unseq` for multithreading.
  auto const image = renderer.render(htracer::rendering::unseq);

  // Save the image in PPM format with 1 byte per value (3 bytes per pixel).
  htracer::outputs::ppm const ppm;
  auto constexpr ppmbpv = htracer::outputs::ppm::bytes_per_value::BPV1;
  ppm.save<ppmbpv>("basic.ppm", image);
}
