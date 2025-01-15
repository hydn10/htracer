#include <htracer/geometries/plane.hpp>
#include <htracer/geometries/sphere.hpp>
#include <htracer/outputs/ppm.hpp>
#include <htracer/raytracing/cameras/sampling.hpp>
#include <htracer/raytracing/lenses/pinhole.hpp>
#include <htracer/raytracing/pixel_samplers/constant.hpp>
#include <htracer/raytracing/policies.hpp>
#include <htracer/staging/scene.hpp>
#include <htracer/vector.hpp>

#include <cstdlib>
#include <numbers>
#include <variant>


using Float = double;


template<typename Float>
using sphere_t = htracer::geometries::sphere<Float>;
template<typename Float>
using plane_t = htracer::geometries::plane<Float>;


static constexpr htracer::staging::material<Float>
get_sphere_material(Float hue)
{
  using htracer::colors::hsl;

  auto const color = hsl<Float>{hue, 0.7, 0.5}.to_srgb().to_linear();
  return htracer::staging::make_solid<Float>(color, 0.125, 0.05, 200, .4);
}


static auto
build_test_scene()
{
  htracer::staging::scene<Float, sphere_t, plane_t> scene;

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

  if (args.size() != 1)
  {
    return EXIT_FAILURE;
  }

  auto const &filename = args[0];

  auto const scene = build_test_scene();

  htracer::v3<Float> const camera_pos(0, 2.7, 3.1);
  htracer::v3<Float> const camera_dir(0, -0.4, -1.);
  htracer::v3<Float> const camera_up(0, 1, 0);

  // Non-random lens and pixel sampler so output is deterministic.
  htracer::raytracing::lenses::pinhole<Float> const lens;
  htracer::raytracing::pixel_samplers::constant<Float> const pixel_sampler;

  htracer::raytracing::cameras::sampling const camera(
      camera_pos, camera_dir, camera_up, 144, 81, 45 * std::numbers::pi_v<Float> / 180, lens, pixel_sampler);

  htracer::utils::randomness randomness;

  auto const render_op = camera.render(htracer::staging::scene_view(scene), 1, randomness);
  auto const image = render_op(htracer::raytracing::unseq);

  htracer::outputs::ppm const ppm;
  auto constexpr ppmbpv = htracer::outputs::ppm::bytes_per_value::BPV1;
  ppm.save<ppmbpv>(filename, image);

  return EXIT_SUCCESS;
}
