#include <htracer_benchmarks/model.hpp>
#include <htracer_benchmarks/suite.hpp>

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>


namespace htracer::benchmarks
{

namespace
{

constexpr std::uint64_t canonical_seed{0x0123456789abcdefULL};
constexpr measurement_configuration quick_measurement{.warmup_count = 1, .repetition_count = 5};


[[nodiscard]]
std::string
precision_id(precision_kind precision)
{
  return precision == precision_kind::f32 ? "f32" : "f64";
}


[[nodiscard]]
std::string
policy_id(policy_kind policy)
{
  return policy == policy_kind::seq ? "seq" : "par";
}


[[nodiscard]]
benchmark_case
make_mixed_deterministic(precision_kind precision, policy_kind policy)
{
  return {
      .id = "quick.v1/mixed/deterministic/" + precision_id(precision) + "/" + policy_id(policy),
      .canonical = true,
      .render =
          {.scene = scene_kind::mixed,
           .rendering = rendering_kind::deterministic,
           .precision = precision,
           .policy = policy,
           .batcher = batcher_kind::column,
           .sensor = sensor_kind::point,
           .lens = lens_kind::pinhole,
           .width = 640,
           .height = 360,
           .geometry_count = std::nullopt,
           .samples_per_pixel = std::nullopt,
           .seed = std::nullopt},
      .measurement = quick_measurement};
}


[[nodiscard]]
benchmark_case
make_mixed_randomized(precision_kind precision, policy_kind policy)
{
  return {
      .id = "quick.v1/mixed/randomized-seeded/" + precision_id(precision) + "/" + policy_id(policy),
      .canonical = true,
      .render =
          {.scene = scene_kind::mixed,
           .rendering = rendering_kind::randomized,
           .precision = precision,
           .policy = policy,
           .batcher = batcher_kind::column,
           .sensor = sensor_kind::uniform,
           .lens = lens_kind::pinhole,
           .width = 320,
           .height = 180,
           .geometry_count = std::nullopt,
           .samples_per_pixel = 8,
           .seed = canonical_seed},
      .measurement = quick_measurement};
}


[[nodiscard]]
benchmark_case
make_rng_probe(bool seeded, policy_kind policy, std::uint32_t samples)
{
  std::string_view const seed_id = seeded ? "seeded" : "unseeded";
  return {
      .id =
          "quick.v1/rng-probe/" + std::string{seed_id} + "/spp" + std::to_string(samples) + "/f64/" + policy_id(policy),
      .canonical = true,
      .render =
          {.scene = scene_kind::rng_probe,
           .rendering = rendering_kind::randomized,
           .precision = precision_kind::f64,
           .policy = policy,
           .batcher = batcher_kind::column,
           .sensor = sensor_kind::uniform,
           .lens = lens_kind::pinhole,
           .width = 640,
           .height = 360,
           .geometry_count = std::nullopt,
           .samples_per_pixel = samples,
           .seed = seeded ? std::optional<std::uint64_t>{canonical_seed} : std::nullopt},
      .measurement = quick_measurement};
}

} // namespace


std::vector<benchmark_case>
make_quick_suite()
{
  std::vector<benchmark_case> cases;
  cases.reserve(16);

  for (auto const precision : {precision_kind::f32, precision_kind::f64})
  {
    for (auto const policy : {policy_kind::seq, policy_kind::par})
    {
      cases.push_back(make_mixed_deterministic(precision, policy));
      cases.push_back(make_mixed_randomized(precision, policy));
    }
  }

  for (auto const seeded : {false, true})
  {
    for (auto const samples : {std::uint32_t{1}, std::uint32_t{16}})
    {
      for (auto const policy : {policy_kind::seq, policy_kind::par})
      {
        cases.push_back(make_rng_probe(seeded, policy, samples));
      }
    }
  }

  return cases;
}

} // namespace htracer::benchmarks
