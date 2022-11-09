#ifndef HTRACER_UTILS_RANDOM_HPP
#define HTRACER_UTILS_RANDOM_HPP


#include <random>


namespace htracer::utils
{

template<typename Float>
class random
{
  std::default_random_engine g_;
  std::uniform_real_distribution<Float> dist_;

public:
  random(Float a, Float b)
      : g_{std::random_device{}()}
      , dist_{a, b}
  {
  }

  Float
  operator()()
  {
    return dist_(g_);
  }
};

} // namespace htracer::utils

#endif