#ifndef HTRACER_UTILS_GENERATOR_PROVIDERS_SEEDED_PROVIDER_HPP
#define HTRACER_UTILS_GENERATOR_PROVIDERS_SEEDED_PROVIDER_HPP


#include <htracer/rendering/random_seed.hpp>
#include <htracer/utils/generator_providers/generator_state.hpp>

#include <cstdint>
#include <random>


namespace htracer::utils::generator_providers::detail_
{

template<typename Generator>
class seeded_provider
{
  rendering::random_seed seed_;

public:
  using generator_type = Generator;

  explicit constexpr seeded_provider(rendering::random_seed seed) noexcept;

  [[nodiscard]]
  owned_generator_state<Generator>
  make_state(std::uint32_t v_idx, std::uint32_t h_idx) const;
};


template<typename Generator>
constexpr seeded_provider<Generator>::seeded_provider(rendering::random_seed seed) noexcept
    : seed_{seed}
{
}


template<typename Generator>
owned_generator_state<Generator>
seeded_provider<Generator>::make_state(std::uint32_t v_idx, std::uint32_t h_idx) const
{
  std::seed_seq seed_sequence{
      static_cast<std::uint32_t>(seed_.value), static_cast<std::uint32_t>(seed_.value >> 32), v_idx, h_idx};

  return owned_generator_state<Generator>{seed_sequence};
}

} // namespace htracer::utils::generator_providers::detail_

#endif
