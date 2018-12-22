#include <raytracer/vector.hpp>
#include <raytracer/line.hpp>
#include <raytracer/sphere.hpp>

#include <iostream>


using v3 = hdn::vector<float, 3>;
using line = hdn::line<float>;
using sphere = hdn::sphere<float>;


int
main()
{
  v3 vec1({2, 3, 0});
  v3 vec2({0, 0, 5});

  line sight{vec2, vec1.normalized()};

  auto prod = cross(vec1, vec2).normalized();
  auto normal = vec1.normalized();

  std::cout << prod << '\n';
}