#ifndef HTRACER_UTILS_GENERATOR_PROVIDERS_THREAD_LOCAL_PROVIDER_HPP
#define HTRACER_UTILS_GENERATOR_PROVIDERS_THREAD_LOCAL_PROVIDER_HPP


#include <random>


namespace htracer::utils::generator_providers::detail_
{

class thread_local_provider
{
public:
  using generator_type = std::default_random_engine;

  inline std::default_random_engine &
  get_generator() const;
};


std::default_random_engine &
thread_local_provider::get_generator() const
{
  thread_local std::default_random_engine generator(std::random_device{}());
  return generator;
}

} // namespace htracer::utils::generator_providers::detail_

#endif
