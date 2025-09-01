#ifndef HTRACER_RENDERING_POLICIES_HPP
#define HTRACER_RENDERING_POLICIES_HPP


#include <execution>


namespace htracer::rendering
{

struct unsequenced_policy
{
  static inline constexpr bool is_parallel = false;
  [[nodiscard]]
  static inline constexpr auto
  get_std_policy() noexcept
  {
    return std::execution::unseq;
  }
};


struct parallel_unsequenced_policy
{
  static inline constexpr bool is_parallel = true;
  [[nodiscard]]
  static inline constexpr auto
  get_std_policy() noexcept
  {
    return std::execution::par_unseq;
  }
};


inline constexpr unsequenced_policy unseq;
inline constexpr parallel_unsequenced_policy par_unseq;

} // namespace htracer::rendering

#endif