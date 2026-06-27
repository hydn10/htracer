#include <htracer/htracer.hpp>

#include <cstdlib>


int
main()
{
  using ht_f64 = htracer::float_traits<double>;

  ht_f64::v3 const vector(1., 2., 3.);

  return decltype(vector)::size == 3 ? EXIT_SUCCESS : EXIT_FAILURE;
}
