#ifndef HTRACER_RAYTRACING_CONCEPTS_HPP
#define HTRACER_RAYTRACING_CONCEPTS_HPP


#include <htracer/geometries/ray.hpp>
#include <htracer/utils/randomness.hpp>

#include <concepts>
#include <utility>


namespace htracer::raytracing
{

template<typename T, typename Float>
concept deterministic_lens = requires(T a) {
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
concept nondeterministic_lens = requires(T a, utils::randomness<> &r) {
  {
    a.get_ray(
        Float{},
        Float{},
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        std::declval<v3<Float>>(),
        r)
  } -> std::same_as<geometries::ray<Float>>;
};


template<typename T, typename Float>
concept deterministic_pixel_sampler = requires(T a) {
  { a.get_coords(uint32_t{}, uint32_t{}) } -> std::same_as<std::pair<Float, Float>>;
};


template<typename T, typename Float>
concept nondeterministic_pixel_sampler = requires(T a, utils::randomness<> &r) {
  { a.get_coords(uint32_t{}, uint32_t{}, r) } -> std::same_as<std::pair<Float, Float>>;
};


template<typename T, typename Float>
concept lens = deterministic_lens<T, Float> || nondeterministic_lens<T, Float>;


template<typename T, typename Float>
concept pixel_sampler = deterministic_pixel_sampler<T, Float> || nondeterministic_pixel_sampler<T, Float>;

} // namespace htracer::raytracing

#endif
