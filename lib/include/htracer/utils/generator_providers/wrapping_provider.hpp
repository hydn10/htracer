#ifndef HTRACER_UTILS_GENERATOR_PROVIDERS_WRAPPING_PROVIDER_HPP
#define HTRACER_UTILS_GENERATOR_PROVIDERS_WRAPPING_PROVIDER_HPP


#include <htracer/utils/generator_providers/generator_state.hpp>

#include <cstdint>


namespace htracer::utils::generator_providers::detail_
{

template<typename Generator>
class wrapping_provider
{
  Generator &generator_;

public:
  using generator_type = Generator;

  wrapping_provider(Generator &g) noexcept;
  [[nodiscard]]
  borrowed_generator_state<Generator>
  make_state(std::uint32_t, std::uint32_t) const noexcept;
};


template<typename Generator>
wrapping_provider<Generator>::wrapping_provider(Generator &g) noexcept
    : generator_{g}
{
}


template<typename Generator>
borrowed_generator_state<Generator>
wrapping_provider<Generator>::make_state(std::uint32_t, std::uint32_t) const noexcept
{
  return borrowed_generator_state<Generator>{generator_};
}

} // namespace htracer::utils::generator_providers::detail_

#endif
