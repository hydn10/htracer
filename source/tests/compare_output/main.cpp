#include <htracer/outputs/ppm.hpp>
#include <htracer/raytracing/camera.hpp>
#include <htracer/raytracing/lenses/point.hpp>
#include <htracer/raytracing/pixel_samplers/constant.hpp>
#include <htracer/scene/scene.hpp>
#include <htracer/vector.hpp>

#include <execution>
#include <variant>
#include <numbers>


using Float = double;


template<typename Float>
using sphere_t = htracer::geometries::sphere<Float>;
template<typename Float>
using plane_t = htracer::geometries::plane<Float>;


constexpr htracer::scene::material<Float>
get_sphere_material(Float hue)
{
  using htracer::colors::hsl;

  const auto color = hsl<Float>{hue, 0.7, 0.5}.to_srgb().to_linear();
  return htracer::scene::make_solid<Float>(color, 0.125, 0.05, 200, .4);
}


int
main()
{
  htracer::scene::scene<Float, sphere_t, plane_t> scene;

  scene.add_light({{-3., 6., 0.}, {1., 1., 1.}, 20});
  scene.add_light({{3., 6., 0.}, {1., 1., 1.}, 10});

  auto const floor_material = htracer::scene::make_solid<Float>({0.2, 0.2, 0.2}, 0.125, 0, 200, .2);
  auto const mirror_material = htracer::scene::make_mirror<Float>(0, 200, 0.92);

  scene.emplace_object<sphere_t>({{-8.5, 2.0, -5.0}, 2.0}, get_sphere_material(310));
  scene.emplace_object<sphere_t>({{-1.5, 1.0, 0.0}, 1.0}, get_sphere_material(42));
  scene.emplace_object<sphere_t>({{1.5, 1.0, 0.0}, 1.0}, get_sphere_material(110));
  scene.emplace_object<sphere_t>({{6.8, 1.0, -3.0}, 1.0}, get_sphere_material(200));

  scene.emplace_object<plane_t>({{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}}, floor_material);
  scene.emplace_object<plane_t>({{0.0, 0.0, -11.0}, {0.0, -0.2, 1.0}}, mirror_material);

  htracer::v3<Float> const camera_pos(0, 2.7, 3.1);
  htracer::v3<Float> const camera_dir(0, -0.4, -1.);
  htracer::v3<Float> const camera_up(0, 1, 0);

  // Non-random lens and pixel sampler so output is deterministic.
  htracer::raytracing::lenses::point<Float> lens;
  htracer::raytracing::pixel_samplers::constant<Float> pixel_sampler;

  htracer::raytracing::camera const cam(
      camera_pos, camera_dir, camera_up, 144, 81, 45 * std::numbers::pi_v<Float> / 180, lens, pixel_sampler);

  auto const image = cam.render(std::execution::par_unseq, htracer::scene::scene_view(scene), 1);

  auto const filename = "ray_compare_test.ppm";

  htracer::outputs::ppm const ppm;
  auto constexpr ppmbpv = htracer::outputs::ppm::bytes_per_value::BPV1;
  ppm.save<ppmbpv>(filename, image);
}
