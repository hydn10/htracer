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
#include <cstdint>
#include <expected>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>


namespace htracer::benchmarks
{

namespace
{

constexpr htracer::rendering::random_seed canonical_seed{0x0123456789abcdefULL};

template<typename Value>
[[nodiscard]]
Value
required(std::expected<Value, std::string_view> result)
{
  if (!result)
  {
    throw std::logic_error{"invalid canonical benchmark value: " + std::string{result.error()}};
  }
  return std::move(*result);
}


[[nodiscard]]
image_extent
extent(std::uint32_t width, std::uint32_t height)
{
  return required(
      image_extent::try_make(required(image_width::try_make(width)), required(image_height::try_make(height))));
}


[[nodiscard]]
htracer::rendering::samples_per_pixel
samples(std::uint32_t value)
{
  return required(htracer::rendering::samples_per_pixel::try_make(value));
}


[[nodiscard]]
measurement_plan
quick_measurement()
{
  return {.warmups = warmup_count{1}, .repetitions = required(repetition_count::try_make(5))};
}


[[nodiscard]]
benchmark_definition
mixed_deterministic(precision_kind precision, policy_kind policy)
{
  return benchmark_definition::deterministic(mixed_scene{}, precision, policy, extent(640, 360), quick_measurement());
}


[[nodiscard]]
benchmark_definition
mixed_randomized(precision_kind precision, policy_kind policy)
{
  return benchmark_definition::randomized(
      mixed_scene{},
      randomized_render{samples(8), canonical_seed},
      precision,
      policy,
      extent(320, 180),
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
      randomized_render{samples, seed},
      precision_kind::f64,
      policy,
      extent(640, 360),
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
      benchmark_case::canonical(rng_probe(std::nullopt, policy_kind::seq, samples(1))),
      benchmark_case::canonical(rng_probe(std::nullopt, policy_kind::par, samples(1))),
      benchmark_case::canonical(rng_probe(std::nullopt, policy_kind::seq, samples(16))),
      benchmark_case::canonical(rng_probe(std::nullopt, policy_kind::par, samples(16))),
      benchmark_case::canonical(rng_probe(canonical_seed, policy_kind::seq, samples(1))),
      benchmark_case::canonical(rng_probe(canonical_seed, policy_kind::par, samples(1))),
      benchmark_case::canonical(rng_probe(canonical_seed, policy_kind::seq, samples(16))),
      benchmark_case::canonical(rng_probe(canonical_seed, policy_kind::par, samples(16)))};

  return benchmarks;
}

} // namespace htracer::benchmarks
