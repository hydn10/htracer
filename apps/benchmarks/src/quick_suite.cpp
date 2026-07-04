#include <htracer_benchmarks/quick_suite.hpp>

#include <htracer/rendering/random_seed.hpp>
#include <htracer/rendering/samples_per_pixel.hpp>
#include <htracer_benchmarks/benchmark_case.hpp>
#include <htracer_benchmarks/benchmark_definition.hpp>
#include <htracer_benchmarks/image_extent.hpp>
#include <htracer_benchmarks/measurement_plan.hpp>
#include <htracer_benchmarks/render_mode.hpp>
#include <htracer_benchmarks/scene_spec.hpp>

#include <array>
#include <optional>
#include <span>


namespace htracer::benchmarks
{

namespace
{

constexpr htracer::rendering::random_seed canonical_seed{0x0123456789abcdefULL};


[[nodiscard]]
measurement_plan
quick_measurement()
{
  return {.warmups = warmup_count{1}, .repetitions = repetition_count::make(5)};
}


[[nodiscard]]
benchmark_definition
mixed_deterministic(precision_kind precision, policy_kind policy)
{
  return benchmark_definition::deterministic(
      mixed_scene{}, precision, policy, image_extent::make(640, 360), quick_measurement());
}


[[nodiscard]]
benchmark_definition
mixed_randomized(precision_kind precision, policy_kind policy)
{
  return benchmark_definition::randomized(
      mixed_scene{},
      randomized_render::make(htracer::rendering::samples_per_pixel{8}, canonical_seed),
      precision,
      policy,
      image_extent::make(320, 180),
      quick_measurement());
}


[[nodiscard]]
benchmark_definition
rng_probe(
    std::optional<htracer::rendering::random_seed> seed,
    policy_kind policy,
    htracer::rendering::samples_per_pixel samples)
{
  return benchmark_definition::randomized(
      rng_probe_scene{},
      randomized_render::make(samples, seed),
      precision_kind::f64,
      policy,
      image_extent::make(640, 360),
      quick_measurement());
}

} // namespace


std::span<benchmark_case const>
quick_suite_catalog::cases()
{
  static std::array const benchmarks{
      benchmark_case::canonical(mixed_deterministic(precision_kind::f32, policy_kind::seq)),
      benchmark_case::canonical(mixed_randomized(precision_kind::f32, policy_kind::seq)),
      benchmark_case::canonical(mixed_deterministic(precision_kind::f32, policy_kind::par)),
      benchmark_case::canonical(mixed_randomized(precision_kind::f32, policy_kind::par)),
      benchmark_case::canonical(mixed_deterministic(precision_kind::f64, policy_kind::seq)),
      benchmark_case::canonical(mixed_randomized(precision_kind::f64, policy_kind::seq)),
      benchmark_case::canonical(mixed_deterministic(precision_kind::f64, policy_kind::par)),
      benchmark_case::canonical(mixed_randomized(precision_kind::f64, policy_kind::par)),
      benchmark_case::canonical(rng_probe(std::nullopt, policy_kind::seq, htracer::rendering::samples_per_pixel{1})),
      benchmark_case::canonical(rng_probe(std::nullopt, policy_kind::par, htracer::rendering::samples_per_pixel{1})),
      benchmark_case::canonical(rng_probe(std::nullopt, policy_kind::seq, htracer::rendering::samples_per_pixel{16})),
      benchmark_case::canonical(rng_probe(std::nullopt, policy_kind::par, htracer::rendering::samples_per_pixel{16})),
      benchmark_case::canonical(rng_probe(canonical_seed, policy_kind::seq, htracer::rendering::samples_per_pixel{1})),
      benchmark_case::canonical(rng_probe(canonical_seed, policy_kind::par, htracer::rendering::samples_per_pixel{1})),
      benchmark_case::canonical(rng_probe(canonical_seed, policy_kind::seq, htracer::rendering::samples_per_pixel{16})),
      benchmark_case::canonical(
          rng_probe(canonical_seed, policy_kind::par, htracer::rendering::samples_per_pixel{16}))};

  return benchmarks;
}

} // namespace htracer::benchmarks
