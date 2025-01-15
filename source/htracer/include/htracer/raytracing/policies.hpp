#ifndef HTRACER_RAYTRACING_POLICIES_HPP
#define HTRACER_RAYTRACING_POLICIES_HPP


#include <execution>


namespace htracer::raytracing
{

enum class policy_style
{
  UNSEQUENCED,
  PARALLEL,
};


struct unsequenced_policy
{
  constexpr static policy_style style = policy_style::UNSEQUENCED;
};

struct parallel_policy
{
  constexpr static policy_style style = policy_style::PARALLEL;
};

inline constexpr unsequenced_policy unseq{};
inline constexpr parallel_policy par{};


template<policy_style PolicyType>
struct policy_traits
{
};

template<>
struct policy_traits<policy_style::UNSEQUENCED>
{
  static auto constexpr std_policy = std::execution::unseq;
};

template<>
struct policy_traits<policy_style::PARALLEL>
{
  static auto constexpr std_policy = std::execution::par_unseq;
};

} // namespace htracer::raytracing

#endif
