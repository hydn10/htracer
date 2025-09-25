#ifndef HTRACER_UTILS_VECTORTRANSFORM_HPP
#define HTRACER_UTILS_VECTORTRANSFORM_HPP


#include <htracer/utils/vector_crtp.hpp>

#include <utility>


namespace htracer::utils
{
namespace detail_
{

template<typename VectorTo, typename Derived, typename Float, std::size_t N, typename F, std::size_t... Is>
constexpr VectorTo
transform_into_impl(vector_crtp<Derived, Float, N> const &v, F f, std::index_sequence<Is...>)
{
  return {f(v[Is])...};
}


template<typename VectorTo, typename VectorFrom, typename F, std::size_t... Is>
constexpr VectorTo
transform_impl(VectorFrom const &v, F &&f, std::index_sequence<Is...>)
{
  return {f(v[Is])...};
}

} // namespace detail_


template<typename VectorTo, typename Derived, typename Float, std::size_t N, typename F>
constexpr VectorTo
transform_into(vector_crtp<Derived, Float, N> const &v, F &&f)
{
  return detail_::transform_into_impl<VectorTo>(v, std::forward<F>(f), std::make_index_sequence<N>());
}


template<typename Vector, typename F>
constexpr Vector
transform(Vector const &v, F &&f)
{
  return detail_::transform_impl<Vector>(v, std::forward<F>(f), std::make_index_sequence<Vector::size>());
}

} // namespace htracer::utils

#endif
