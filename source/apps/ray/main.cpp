#include <htracer/outputs/ppm.hpp>
#include <htracer/raytracing/cameras/sampling.hpp>
#include <htracer/raytracing/lenses/pinhole.hpp>
#include <htracer/raytracing/lenses/point.hpp>
#include <htracer/raytracing/pixel_samplers/constant.hpp>
#include <htracer/raytracing/pixel_samplers/uniform.hpp>
#include <htracer/raytracing/sampler.hpp>
#include <htracer/scene/scene.hpp>
#include <htracer/vector.hpp>

#include <execution>
#include <numbers>
#include <variant>


using Float = double;


constexpr htracer::scene::material<Float>
get_sphere_material(Float hue)
{
  using htracer::colors::hsl;

  auto const color = hsl<Float>{hue, 0.7, 0.5}.to_srgb().to_linear();
  return htracer::scene::make_solid<Float>(color, 0.125, 0.05, 200, .4);
}


template<typename Float>
using sphere_t = htracer::geometries::sphere<Float>;
template<typename Float>
using plane_t = htracer::geometries::plane<Float>;


int
main(int argc, char const *argv[])
{
  std::vector<std::string_view> const args(argv + 1, argv + argc);

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

  auto const focal_vec = htracer::v3<Float>(0.0, 1.0, 0.0) - camera_pos;
  // auto const focal_distance = std::sqrt(dot(focal_vec, focal_vec));

  // htracer::v3<Float> const camera_pos(8, 4, 0.4);
  // htracer::v3<Float> const camera_dir = htracer::v3<Float>(1.5, 1.0, -3.0) - camera_pos;
  // htracer::v3<Float> const camera_up(0, 1, 0);

  htracer::raytracing::lenses::point<Float> const lens;
  // htracer::raytracing::lenses::pinhole<Float> lens(0.5, 3);

  htracer::raytracing::pixel_samplers::constant<Float> const pixel_sampler;
  // htracer::raytracing::pixel_samplers::uniform<Float> pixel_sampler;

  htracer::raytracing::cameras::sampling const camera(
      camera_pos, camera_dir, camera_up, 1024, 576, 45 * std::numbers::pi_v<Float> / 180, lens, pixel_sampler);

  htracer::utils::randomness rand;

  auto const render_op = camera.render(htracer::scene::scene_view(scene), 1, rand);
  auto const image = render_op(htracer::raytracing::unseq);

  auto const filename = args.size() > 0 ? args[0] : "out.ppm";

  htracer::outputs::ppm const ppm;
  auto constexpr ppmbpv = htracer::outputs::ppm::bytes_per_value::BPV1;
  ppm.save<ppmbpv>(filename, image);
}
