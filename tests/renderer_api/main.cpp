#include <htracer/htracer.hpp>

#include <cstdint>
#include <cstdlib>
#include <execution>
#include <functional>
#include <numbers>
#include <random>
#include <utility>


using ht_f64 = htracer::float_traits<double>;


namespace
{

template<typename Float>
class borrowed_sensor
{
  int const *marker_;

public:
  explicit constexpr borrowed_sensor(int const &marker) noexcept
      : marker_{&marker}
  {
  }

  [[nodiscard]]
  constexpr std::pair<Float, Float>
  get_coords(uint32_t v_idx, uint32_t h_idx) const noexcept
  {
    return {v_idx + static_cast<Float>(*marker_ == 0), h_idx};
  }
};


template<typename Float>
class mutable_only_sensor
{
public:
  [[nodiscard]]
  constexpr std::pair<Float, Float>
  get_coords(uint32_t v_idx, uint32_t h_idx) noexcept
  {
    return {v_idx, h_idx};
  }
};


class alternate_engine
{
  std::minstd_rand engine_;

public:
  using result_type = std::minstd_rand::result_type;

  [[nodiscard]]
  static constexpr result_type
  min() noexcept;

  [[nodiscard]]
  static constexpr result_type
  max() noexcept;

  result_type
  operator()() noexcept;
};


struct unsupported_policy
{
};


template<typename Float>
class alternate_engine_sensor
{
public:
  [[nodiscard]]
  constexpr std::pair<Float, Float>
  get_coords(uint32_t v_idx, uint32_t h_idx, alternate_engine &generator) const noexcept;
};


constexpr alternate_engine::result_type
alternate_engine::min() noexcept
{
  return std::minstd_rand::min();
}


constexpr alternate_engine::result_type
alternate_engine::max() noexcept
{
  return std::minstd_rand::max();
}


alternate_engine::result_type
alternate_engine::operator()() noexcept
{
  return engine_();
}


template<typename Float>
constexpr std::pair<Float, Float>
alternate_engine_sensor<Float>::get_coords(
    uint32_t v_idx, uint32_t h_idx, [[maybe_unused]] alternate_engine &generator) const noexcept
{
  return {v_idx, h_idx};
}


[[nodiscard]]
ht_f64::camera
make_camera()
{
  return {{0, 0, 1}, {0, 0, -1}, {0, 1, 0}, 2, 2, 45 * std::numbers::pi / 180};
}


[[nodiscard]]
ht_f64::scene
make_scene()
{
  return {};
}

} // namespace


using deterministic_renderer_type = decltype(htracer::rendering::make_renderer(
    std::declval<ht_f64::camera>(),
    std::declval<htracer::rendering::batchers::column_batcher>(),
    std::declval<ht_f64::point_sensor>(),
    std::declval<ht_f64::pinhole_lens>()));

using randomized_renderer_type = decltype(htracer::rendering::make_renderer(
    std::declval<ht_f64::camera>(),
    std::declval<htracer::rendering::batchers::column_batcher>(),
    std::declval<ht_f64::uniform_sensor>(),
    std::declval<ht_f64::pinhole_lens>()));

template<typename Renderer, typename Policy>
concept renders_without_samples_with_policy =
    requires(Renderer const &renderer, ht_f64::scene const &scene, Policy policy) { renderer.render(policy, scene); };

template<typename Renderer, typename Policy>
concept renders_with_samples_with_policy =
    requires(Renderer const &renderer, ht_f64::scene const &scene, Policy policy) {
      renderer.render(policy, scene, htracer::rendering::samples_per_pixel{1});
    };

template<typename Renderer>
concept renders_without_samples = requires(Renderer const &renderer, ht_f64::scene const &scene) {
  renderer.render(htracer::rendering::seq, scene);
  renderer.render(htracer::rendering::par, scene);
};

template<typename Renderer>
concept renders_with_samples = requires(Renderer const &renderer, ht_f64::scene const &scene) {
  renderer.render(htracer::rendering::seq, scene, htracer::rendering::samples_per_pixel{1});
  renderer.render(htracer::rendering::par, scene, htracer::rendering::samples_per_pixel{1});
};

template<typename Renderer>
concept renders_with_seed = requires(Renderer const &renderer, ht_f64::scene const &scene) {
  renderer.render(
      htracer::rendering::seq, scene, htracer::rendering::samples_per_pixel{1}, htracer::rendering::random_seed{1});
  renderer.render(
      htracer::rendering::par, scene, htracer::rendering::samples_per_pixel{1}, htracer::rendering::random_seed{1});
};

template<typename Sensor>
concept one_shot_renders_without_samples = requires(ht_f64::scene const &scene, ht_f64::camera camera) {
  htracer::rendering::render(
      htracer::rendering::seq,
      scene,
      camera,
      htracer::rendering::batchers::column_batcher{},
      Sensor{},
      ht_f64::pinhole_lens{});
};

template<typename Sensor>
concept one_shot_renders_with_samples = requires(ht_f64::scene const &scene, ht_f64::camera camera) {
  htracer::rendering::render(
      htracer::rendering::seq,
      scene,
      camera,
      htracer::rendering::batchers::column_batcher{},
      Sensor{},
      ht_f64::pinhole_lens{},
      htracer::rendering::samples_per_pixel{1});
};

template<typename Sensor>
concept one_shot_renders_with_seed = requires(ht_f64::scene const &scene, ht_f64::camera camera) {
  htracer::rendering::render(
      htracer::rendering::seq,
      scene,
      camera,
      htracer::rendering::batchers::column_batcher{},
      Sensor{},
      ht_f64::pinhole_lens{},
      htracer::rendering::samples_per_pixel{1},
      htracer::rendering::random_seed{1});
  htracer::rendering::render(
      htracer::rendering::par,
      scene,
      camera,
      htracer::rendering::batchers::column_batcher{},
      Sensor{},
      ht_f64::pinhole_lens{},
      htracer::rendering::samples_per_pixel{1},
      htracer::rendering::random_seed{1});
};

static_assert(htracer::rendering::deterministic_sensor<ht_f64::point_sensor, double>);
static_assert(!htracer::rendering::deterministic_sensor<mutable_only_sensor<double>, double>);
static_assert(std::uniform_random_bit_generator<alternate_engine>);
static_assert(htracer::rendering::nondeterministic_sensor<alternate_engine_sensor<double>, double, alternate_engine>);
static_assert(!htracer::rendering::nondeterministic_sensor<alternate_engine_sensor<double>, double>);
static_assert(htracer::rendering::sensor<alternate_engine_sensor<double>, double, alternate_engine>);
static_assert(htracer::rendering::rendering_policy<decltype(htracer::rendering::seq)>);
static_assert(htracer::rendering::rendering_policy<decltype(htracer::rendering::par)>);
static_assert(htracer::rendering::rendering_policy<decltype(std::execution::seq)>);
static_assert(htracer::rendering::rendering_policy<decltype(std::execution::par)>);
static_assert(!htracer::rendering::rendering_policy<unsupported_policy>);
static_assert(!htracer::rendering::rendering_policy<int>);

static_assert(!renders_without_samples_with_policy<deterministic_renderer_type, unsupported_policy>);
static_assert(!renders_with_samples_with_policy<randomized_renderer_type, unsupported_policy>);

static_assert(renders_without_samples<deterministic_renderer_type>);
static_assert(!renders_with_samples<deterministic_renderer_type>);
static_assert(!renders_with_seed<deterministic_renderer_type>);

static_assert(!renders_without_samples<randomized_renderer_type>);
static_assert(renders_with_samples<randomized_renderer_type>);
static_assert(renders_with_seed<randomized_renderer_type>);

static_assert(one_shot_renders_without_samples<ht_f64::point_sensor>);
static_assert(!one_shot_renders_with_samples<ht_f64::point_sensor>);
static_assert(!one_shot_renders_without_samples<ht_f64::uniform_sensor>);
static_assert(one_shot_renders_with_samples<ht_f64::uniform_sensor>);
static_assert(!one_shot_renders_with_seed<ht_f64::point_sensor>);
static_assert(one_shot_renders_with_seed<ht_f64::uniform_sensor>);


int
main() // NOLINT(bugprone-exception-escape)
{
  auto const scene = make_scene();

  htracer::rendering::batchers::column_batcher const batcher;
  ht_f64::point_sensor const sensor;
  ht_f64::pinhole_lens const lens;

  auto const lvalue_renderer = htracer::rendering::make_renderer(make_camera(), batcher, sensor, lens);
  auto const lvalue_image = lvalue_renderer.render(htracer::rendering::seq, scene);

  auto const temporary_renderer = htracer::rendering::make_renderer(
      make_camera(), htracer::rendering::batchers::column_batcher{}, ht_f64::point_sensor{}, ht_f64::pinhole_lens{});
  auto const temporary_image = temporary_renderer.render(htracer::rendering::seq, scene);

  int const marker = 1;
  borrowed_sensor<double> const borrowed{marker};

  auto const borrowed_renderer = htracer::rendering::make_renderer(
      make_camera(), htracer::rendering::batchers::column_batcher{}, std::cref(borrowed), ht_f64::pinhole_lens{});
  auto const borrowed_image = borrowed_renderer.render(htracer::rendering::seq, scene);

  auto const one_shot_image = htracer::rendering::render(
      htracer::rendering::seq,
      scene,
      make_camera(),
      htracer::rendering::batchers::column_batcher{},
      ht_f64::point_sensor{},
      ht_f64::pinhole_lens{});

  if (lvalue_image.pixels().size() != 4 || temporary_image.pixels().size() != 4 ||
      borrowed_image.pixels().size() != 4 || one_shot_image.pixels().size() != 4)
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
