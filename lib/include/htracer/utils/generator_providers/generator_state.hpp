#ifndef HTRACER_UTILS_GENERATOR_PROVIDERS_GENERATOR_STATE_HPP
#define HTRACER_UTILS_GENERATOR_PROVIDERS_GENERATOR_STATE_HPP


#include <random>
#include <utility>


namespace htracer::utils::generator_providers::detail_
{

template<typename Generator>
class borrowed_generator_state
{
  Generator *generator_;

public:
  explicit borrowed_generator_state(Generator &generator) noexcept;

  borrowed_generator_state(borrowed_generator_state const &) = delete;
  borrowed_generator_state &
  operator=(borrowed_generator_state const &) = delete;

  borrowed_generator_state(borrowed_generator_state &&other) noexcept;
  borrowed_generator_state &
  operator=(borrowed_generator_state &&other) noexcept;

  [[nodiscard]]
  Generator &
  get() & noexcept;

  Generator &
  get() && = delete;
};


template<typename Generator>
class owned_generator_state
{
  Generator generator_;

public:
  explicit owned_generator_state(std::seed_seq &seed_sequence);

  owned_generator_state(owned_generator_state const &) = delete;
  owned_generator_state &
  operator=(owned_generator_state const &) = delete;

  owned_generator_state(owned_generator_state &&);
  owned_generator_state &
  operator=(owned_generator_state &&);

  [[nodiscard]]
  Generator &
  get() & noexcept;

  Generator &
  get() && = delete;
};


template<typename Generator>
borrowed_generator_state<Generator>::borrowed_generator_state(Generator &generator) noexcept
    : generator_{&generator}
{
}


template<typename Generator>
borrowed_generator_state<Generator>::borrowed_generator_state(borrowed_generator_state<Generator> &&other) noexcept
    : generator_{std::exchange(other.generator_, nullptr)}
{
}


template<typename Generator>
borrowed_generator_state<Generator> &
borrowed_generator_state<Generator>::operator=(borrowed_generator_state<Generator> &&other) noexcept
{
  generator_ = std::exchange(other.generator_, nullptr);
  return *this;
}


template<typename Generator>
Generator &
borrowed_generator_state<Generator>::get() & noexcept
{
  return *generator_;
}


template<typename Generator>
owned_generator_state<Generator>::owned_generator_state(std::seed_seq &seed_sequence)
    : generator_{seed_sequence}
{
}


template<typename Generator>
owned_generator_state<Generator>::owned_generator_state(owned_generator_state<Generator> &&) = default;


template<typename Generator>
owned_generator_state<Generator> &
owned_generator_state<Generator>::operator=(owned_generator_state<Generator> &&) = default;


template<typename Generator>
Generator &
owned_generator_state<Generator>::get() & noexcept
{
  return generator_;
}

} // namespace htracer::utils::generator_providers::detail_

#endif
