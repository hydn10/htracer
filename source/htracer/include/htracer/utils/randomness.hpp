#ifndef HTRACER_UTILS_RANDOMNESS_HPP
#define HTRACER_UTILS_RANDOMNESS_HPP


#include <random>


namespace htracer::utils
{

template<typename T>
concept uniform_random_generator = requires(T a) {
  { a() } -> std::same_as<typename T::result_type>;
  requires std::is_unsigned_v<typename T::result_type>;
  requires std::numeric_limits<typename T::result_type>::min() == 0;
  requires std::numeric_limits<typename T::result_type>::max() > 0;
};


template<uniform_random_generator Engine = std::default_random_engine>
class randomness
{
  Engine engine_;

public:
  randomness();

  randomness(randomness const &other) = delete;
  randomness &
  operator=(randomness const &other) = delete;
  randomness(randomness &&other) = delete;
  randomness &
  operator=(randomness &&other) = delete;

  template<typename Distribution>
  decltype(auto)
  operator()(Distribution &dist);
};


template<uniform_random_generator Engine>
randomness<Engine>::randomness()
    : engine_{std::random_device{}()}
{
}


template<uniform_random_generator Engine>
template<typename Distribution>
decltype(auto)
randomness<Engine>::operator()(Distribution &dist)
{
  return dist(engine_);
}

} // namespace htracer::utils

#endif
