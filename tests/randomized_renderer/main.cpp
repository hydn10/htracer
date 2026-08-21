#include <htracer/htracer.hpp>
#include <htracer/utils/generator_providers/generator_state.hpp>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <numbers>
#include <type_traits>
#include <utility>


using ht_f64 = htracer::float_traits<double>;

using borrowed_generator_state =
    htracer::utils::generator_providers::detail_::borrowed_generator_state<htracer::rendering::random_engine>;
using owned_generator_state =
    htracer::utils::generator_providers::detail_::owned_generator_state<htracer::rendering::random_engine>;


template<typename State>
concept generator_accessible_as_rvalue = requires(State state) { std::move(state).get(); };


static_assert(!std::is_copy_constructible_v<borrowed_generator_state>);
static_assert(std::is_move_constructible_v<borrowed_generator_state>);
static_assert(!generator_accessible_as_rvalue<borrowed_generator_state>);

static_assert(!std::is_copy_constructible_v<owned_generator_state>);
static_assert(std::is_move_constructible_v<owned_generator_state>);
static_assert(!generator_accessible_as_rvalue<owned_generator_state>);


namespace
{

[[nodiscard]]
ht_f64::scene
make_scene()
{
  ht_f64::scene scene;
  scene.add_light({.position = {-2, 3, 1}, .color = {1, 1, 1}, .intensity = 10});
  auto const material = ht_f64::make_solid_material({0.4, 0.2, 0.1}, 0.1, 0.1, 20, 0);
  scene.emplace_sphere({{0, 0, -3}, 1}, material);
  return scene;
}


[[nodiscard]]
bool
valid_image(ht_f64::image const &image)
{
  if (image.h_res() != 8 || image.v_res() != 6 || image.pixels().size() != 48)
  {
    return false;
  }

  return std::ranges::all_of(
      image.pixels(),
      [](auto const &pixel)
      {
        return std::ranges::all_of(
            pixel,
            [](auto value)
            {
              return std::isfinite(value);
            });
      });
}


[[nodiscard]]
bool
exactly_equal(ht_f64::image const &lhs, ht_f64::image const &rhs)
{
  if (lhs.h_res() != rhs.h_res() || lhs.v_res() != rhs.v_res() || lhs.pixels().size() != rhs.pixels().size())
  {
    return false;
  }

  return std::ranges::equal(
      lhs.pixels(),
      rhs.pixels(),
      [](auto const &lhs_pixel, auto const &rhs_pixel)
      {
        return std::ranges::equal(lhs_pixel, rhs_pixel);
      });
}

} // namespace


int
main() // NOLINT(bugprone-exception-escape)
{
  auto const scene = make_scene();
  ht_f64::camera const camera{{0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 8, 6, 45 * std::numbers::pi / 180};
  auto const renderer = htracer::rendering::make_renderer(
      camera, htracer::rendering::batchers::pixel_batcher{}, ht_f64::uniform_sensor{}, ht_f64::pinhole_lens{});
  auto constexpr samples = htracer::rendering::samples_per_pixel{4};
  auto constexpr seed = htracer::rendering::random_seed{0x0123456789ABCDEFull};

  auto const unseeded_seq = renderer.render(htracer::rendering::seq, scene, samples);
  auto const unseeded_par = renderer.render(htracer::rendering::par, scene, samples);
  auto const seeded_seq_first = renderer.render(htracer::rendering::seq, scene, samples, seed);
  auto const seeded_seq_second = renderer.render(htracer::rendering::seq, scene, samples, seed);
  auto const seeded_par = renderer.render(htracer::rendering::par, scene, samples, seed);

  auto const valid = valid_image(unseeded_seq) && valid_image(unseeded_par) && valid_image(seeded_seq_first) &&
                     valid_image(seeded_seq_second) && valid_image(seeded_par);
  auto const reproducible = exactly_equal(seeded_seq_first, seeded_seq_second);
  auto const policy_independent = exactly_equal(seeded_seq_first, seeded_par);

  return valid && reproducible && policy_independent ? EXIT_SUCCESS : EXIT_FAILURE;
}
