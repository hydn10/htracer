#ifndef HTRACER_UTILS_VECTORTRANSFORM_HPP
#define HTRACER_UTILS_VECTORTRANSFORM_HPP


#include <utility>


namespace htracer::utils
{
namespace detail_
{
template<typename VectorTo, typename VectorFrom, typename F, std::size_t... Is>
VectorTo
transform_impl(const VectorFrom v, F f, std::index_sequence<Is...>)
{
  return {f(v[Is])...};
}
} // namespace detail_


template<typename VectorTo, typename VectorFrom, typename F>
VectorTo
transform_into(const VectorFrom& v, F f)
{
  return detail_::transform_impl<VectorTo>(
      v, f, std::make_index_sequence<VectorFrom::size>());
}


template<typename Vector, typename F>
Vector
transform(const Vector& v, F f)
{
  return detail_::transform_impl<Vector>(
      v, f, std::make_index_sequence<Vector::size>());
}

} // namespace htracer::utils

#endif // HTRACER_UTILS_VECTORTRANSFORM_HPP
