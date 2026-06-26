#ifndef HTRACER_RENDERING_DETAIL_COMPONENT_REF_HPP
#define HTRACER_RENDERING_DETAIL_COMPONENT_REF_HPP


#include <functional>
#include <type_traits>


namespace htracer::rendering::detail_
{

template<typename T>
struct unwrap_reference_wrapper
{
  using type = T;
};


template<typename T>
struct unwrap_reference_wrapper<std::reference_wrapper<T>>
{
  using type = T;
};


template<typename T>
using stored_component = std::decay_t<T>;


template<typename T>
using component_type = typename unwrap_reference_wrapper<std::decay_t<T>>::type;


template<typename T>
constexpr T &
component_ref(T &component) noexcept
{
  return component;
}


template<typename T>
constexpr T const &
component_ref(T const &component) noexcept
{
  return component;
}


template<typename T>
constexpr T &
component_ref(std::reference_wrapper<T> const &component) noexcept
{
  return component.get();
}

} // namespace htracer::rendering::detail_

#endif
