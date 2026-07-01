#ifndef HTRACER_RENDERING_CONCEPTS_HPP
#define HTRACER_RENDERING_CONCEPTS_HPP


#include <htracer/geometries/ray.hpp>
#include <htracer/rendering/random_engine.hpp>

#include <concepts>
#include <cstdint>
#include <utility>


namespace htracer::rendering
{

template<typename T, typename Float>
concept deterministic_lens = requires(T const a) {
  {
    a.get_ray(
        Float{},
        Float{},
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>())
  } -> std::same_as<geometries::ray<Float>>;
};


template<typename T, typename Float>
concept nondeterministic_lens = requires(T const a, random_engine &g) {
  {
    a.get_ray(
        Float{},
        Float{},
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        g)
  } -> std::same_as<geometries::ray<Float>>;
};


template<typename T, typename Float>
concept deterministic_sensor = requires(T const a) {
  { a.get_coords(uint32_t{}, uint32_t{}) } -> std::same_as<std::pair<Float, Float>>;
};


template<typename T, typename Float>
concept nondeterministic_sensor = requires(T const a, random_engine &g) {
  { a.get_coords(uint32_t{}, uint32_t{}, g) } -> std::same_as<std::pair<Float, Float>>;
};


template<typename T, typename Float>
concept lens = deterministic_lens<T, Float> || nondeterministic_lens<T, Float>;


template<typename T, typename Float>
concept sensor = deterministic_sensor<T, Float> || nondeterministic_sensor<T, Float>;

} // namespace htracer::rendering

#endif
