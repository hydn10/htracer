#ifndef HTRACER_UTILS_GENERATOR_PROVIDERS_THREAD_LOCAL_PROVIDER_HPP
#define HTRACER_UTILS_GENERATOR_PROVIDERS_THREAD_LOCAL_PROVIDER_HPP


#include <htracer/utils/generator_providers/generator_state.hpp>

#include <cstdint>
#include <random>


namespace htracer::utils::generator_providers::detail_
{

template<typename Generator>
class thread_local_provider
{
public:
  using generator_type = Generator;

  [[nodiscard]]
  borrowed_generator_state<Generator>
  make_state(std::uint32_t, std::uint32_t) const;
};


template<typename Generator>
borrowed_generator_state<Generator>
thread_local_provider<Generator>::make_state(std::uint32_t, std::uint32_t) const
{
  thread_local Generator generator(std::random_device{}());
  return borrowed_generator_state<Generator>{generator};
}

} // namespace htracer::utils::generator_providers::detail_

#endif
