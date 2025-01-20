#ifndef HTRACER_UTILS_GENERATOR_PROVIDERS_WRAPPING_PROVIDER_HPP
#define HTRACER_UTILS_GENERATOR_PROVIDERS_WRAPPING_PROVIDER_HPP


#include <random>


namespace htracer::utils::generator_providers::detail_
{

class wrapping_provider
{
  std::default_random_engine &generator_;

public:
  using generator_type = std::default_random_engine;

  wrapping_provider(std::default_random_engine &g) noexcept;
  std::default_random_engine &
  get_generator() const noexcept;
};


wrapping_provider::wrapping_provider(std::default_random_engine &g) noexcept
    : generator_{g}
{
}


std::default_random_engine &
wrapping_provider::get_generator() const noexcept
{
  return generator_;
}

} // namespace htracer::utils::generator_providers::detail_

#endif
