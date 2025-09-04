#include <htracer/float_traits.hpp>

#include <cmath>
#include <numbers>
#include <ranges>


using ht_f64 = htracer::float_traits<double>;


double
degs_to_rads(double degs)
{
  return degs * std::numbers::pi / 180;
}


constexpr auto
make_ring_positions_range(unsigned size, double radius, double azimuth_offset_degs, double tilt_degs, double spin_degs)
{
  return std::views::iota(0u, size) | std::views::transform([=](auto idx)
  {
    using vec = typename ht_f64::v3;
    using std::sin;
    using std::cos;

    auto const degs = idx * 2 * std::numbers::pi / size;

    auto const azimuth = degs + degs_to_rads(azimuth_offset_degs);

    auto const tilt = degs_to_rads(tilt_degs);
    auto const spin = degs_to_rads(spin_degs);

    auto const flat_pos = radius * vec{cos(azimuth), 0, sin(azimuth)};
    auto const tilted = vec{flat_pos[0], -flat_pos[2] * sin(tilt), flat_pos[2] * cos(tilt)};
    return vec{
        tilted[0] * cos(spin) + tilted[2] * sin(spin), tilted[1], tilted[0] * -sin(spin) + tilted[2] * cos(spin)};
  });
}


int
main()
{
  ht_f64::scene scene;

  scene.add_light({{0, 20, -1}, {1., 1., 1.}, 50});

  auto const center_ball_material = ht_f64::make_solid_material({0.02, 0.02, 0.02}, 0.05, 0, 0, .5);
  auto const floor_material = ht_f64::make_solid_material({1, 1, 1}, 0.15, 0, 200, .45);
  auto const back_wall_material = ht_f64::make_solid_material({0.05, 0.05, 0.05}, 0.08, 0, 10, .02);
  auto const side_wall_material = ht_f64::make_solid_material(
      ht_f64::hsl{52, 0.9, 0.5}.to_srgb().to_linear(), .3, 0, 0.2, .2);

  auto const make_sphere_material = [](double hue)
  {
    // Wrap hue to [0, 360].
    hue = std::fmod((std::fmod(hue, 360.0) + 360.0), 360.0);

    auto const color = ht_f64::hsl{hue, 0.9, 0.5}.to_srgb().to_linear();
    return ht_f64::make_solid_material(color, 0.3, 0.05, 200, .1);
  };

  unsigned constexpr num_inner_balls = 12;

  for (auto const &[idx, pos] : make_ring_positions_range(num_inner_balls, 2, 10, 36, 54) | std::views::enumerate)
  {
    unsigned constexpr hue_from = 220;
    unsigned constexpr hue_to = 360;

    auto const hue = hue_from + (static_cast<double>(idx) * (hue_to - hue_from)) / num_inner_balls;
    scene.emplace_sphere({pos, 0.3}, make_sphere_material(hue));
  }

  scene.emplace_sphere({{0, 0, 0}, 1}, center_ball_material);
  scene.emplace_plane({{0, -2, 0}, {0, 1, 0}}, floor_material);
  scene.emplace_plane({{0, 0, -5}, {0, 0, 1}}, back_wall_material);
  scene.emplace_plane({{-20, 0, 0}, {1, 0, -.1}}, side_wall_material);
  scene.emplace_plane({{20, 0, 0}, {-1, 0, -.1}}, side_wall_material);

  ht_f64::v3 const cam_pos(0, 0.2, 3.2);
  ht_f64::v3 const cam_view(0, -0.75, -3.2);
  ht_f64::v3 const cam_up(0, 1, 0);

  double const fov_degrees = 40;
  double const fov_rads = fov_degrees * std::numbers::pi / 180;

  ht_f64::camera const camera(cam_pos, cam_view, cam_up, 3000, 600, fov_rads);

  htracer::rendering::batchers::column_batcher batcher;

  ht_f64::uniform_sensor const sensor;
  ht_f64::thin_lens const lens(.05, 3.2);


  auto const renderer = htracer::rendering::make_renderer(batcher, scene, camera, sensor, lens);
  auto const image = renderer.render(htracer::rendering::par_unseq, 200);

  htracer::outputs::ppm const ppm;
  auto constexpr ppmbpv = htracer::outputs::ppm::bytes_per_value::BPV2;
  ppm.save<ppmbpv>("disco_ring.ppm", image);
}
