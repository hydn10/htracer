#include <another.hpp>

#include <htracer/htracer.hpp>


int
main()
{
  using ht_f64 = htracer::float_traits<double>;

  ht_f64::v3 const vec(1., 2., 3.);

  return static_cast<int>(decltype(vec)::size - another::get_v3_size());
}
