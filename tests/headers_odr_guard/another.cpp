#include <another.hpp>

#include <htracer/htracer.hpp>

#include <cstddef>


namespace another
{

size_t
get_v3_size()
{
  using ht_f64 = htracer::float_traits<double>;

  ht_f64::v3 const vec(1., 2., 3.);

  return decltype(vec)::size;
}

} // namespace another
