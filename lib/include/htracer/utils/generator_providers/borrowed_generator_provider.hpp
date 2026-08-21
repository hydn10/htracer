#ifndef HTRACER_UTILS_GENERATOR_PROVIDERS_BORROWED_GENERATOR_PROVIDER_HPP
#define HTRACER_UTILS_GENERATOR_PROVIDERS_BORROWED_GENERATOR_PROVIDER_HPP


#include <htracer/utils/generator_providers/generator_state.hpp>

#include <cstdint>
#include <functional>


namespace htracer::utils::generator_providers::detail_
{

template<typename Generator>
class borrowed_generator_provider
{
  std::reference_wrapper<Generator> generator_;

public:
  using generator_type = Generator;

  explicit borrowed_generator_provider(Generator &generator) noexcept;

  [[nodiscard]]
  borrowed_generator_state<Generator>
  make_state(std::uint32_t /*unused*/, std::uint32_t /*unused*/) const noexcept;
};


template<typename Generator>
borrowed_generator_provider<Generator>::borrowed_generator_provider(Generator &generator) noexcept
    : generator_{generator}
{
}


template<typename Generator>
borrowed_generator_state<Generator>
borrowed_generator_provider<Generator>::make_state(
    std::uint32_t /*unused*/, std::uint32_t /*unused*/) const noexcept
{
  return borrowed_generator_state<Generator>{generator_.get()};
}

} // namespace htracer::utils::generator_providers::detail_

#endif
