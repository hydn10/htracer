#ifndef HTRACER_RAYTRACING_POLICIES_HPP
#define HTRACER_RAYTRACING_POLICIES_HPP


namespace htracer::raytracing
{

enum class policy_type
{
  UNSEQUENCED,
  PARALLEL,
};

struct unsequenced_policy
{
  constexpr static policy_type policy_type = policy_type::UNSEQUENCED;
};
struct parallel_policy
{
  constexpr static policy_type policy_type = policy_type::PARALLEL;
};

inline constexpr unsequenced_policy unseq{};
inline constexpr parallel_policy par{};

} // namespace htracer::raytracing

#endif
