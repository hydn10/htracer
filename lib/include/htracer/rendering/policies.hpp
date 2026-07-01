#ifndef HTRACER_RENDERING_POLICIES_HPP
#define HTRACER_RENDERING_POLICIES_HPP


#include <execution>


namespace htracer::rendering
{

struct sequenced_policy
{
  static inline constexpr bool is_parallel = false;
  [[nodiscard]]
  static inline constexpr auto
  get_std_policy() noexcept
  {
    return std::execution::seq;
  }
};


struct parallel_policy
{
  static inline constexpr bool is_parallel = true;
  [[nodiscard]]
  static inline constexpr auto
  get_std_policy() noexcept
  {
    return std::execution::par;
  }
};


inline constexpr sequenced_policy seq;
inline constexpr parallel_policy par;

} // namespace htracer::rendering

#endif
