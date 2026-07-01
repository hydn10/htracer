#ifndef HTRACER_RENDERING_POLICIES_HPP
#define HTRACER_RENDERING_POLICIES_HPP


#include <concepts>
#include <execution>
#include <type_traits>


namespace htracer::rendering
{

using sequenced_policy = std::execution::sequenced_policy;
using parallel_policy = std::execution::parallel_policy;


inline constexpr sequenced_policy seq = std::execution::seq;
inline constexpr parallel_policy par = std::execution::par;


template<typename T>
concept rendering_policy =
    std::same_as<std::remove_cvref_t<T>, sequenced_policy> || std::same_as<std::remove_cvref_t<T>, parallel_policy>;

} // namespace htracer::rendering

#endif
