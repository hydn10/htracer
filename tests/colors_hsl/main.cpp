#include <htracer/colors/colors.hpp>

#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>


namespace
{

using hsl = htracer::colors::hsl<double>;
using rgb = std::array<double, 3>;


bool
check(hsl const &input, rgb const &expected)
{
  auto const actual = input.to_srgb();
  constexpr double tolerance = 1e-12;

  if (std::abs(actual.r() - expected[0]) <= tolerance && std::abs(actual.g() - expected[1]) <= tolerance
      && std::abs(actual.b() - expected[2]) <= tolerance)
  {
    return true;
  }

  std::cerr << "HSL(" << input.h() << ", " << input.s() << ", " << input.l() << ") produced RGB(" << actual.r()
            << ", " << actual.g() << ", " << actual.b() << "), expected RGB(" << expected[0] << ", "
            << expected[1] << ", " << expected[2] << ")\n";
  return false;
}

} // namespace


int
main()
{
  bool success = true;

  success &= check({0, 0, 0}, {0, 0, 0});
  success &= check({60, 0, 0.25}, {0.25, 0.25, 0.25});
  success &= check({180, 0, 0.5}, {0.5, 0.5, 0.5});
  success &= check({300, 0, 0.75}, {0.75, 0.75, 0.75});
  success &= check({120, 0, 1}, {1, 1, 1});

  success &= check({0, 1, 0.5}, {1, 0, 0});
  success &= check({60, 1, 0.5}, {1, 1, 0});
  success &= check({120, 1, 0.5}, {0, 1, 0});
  success &= check({180, 1, 0.5}, {0, 1, 1});
  success &= check({240, 1, 0.5}, {0, 0, 1});
  success &= check({300, 1, 0.5}, {1, 0, 1});

  success &= check({0, 0.5, 0.5}, {0.75, 0.25, 0.25});
  success &= check({30, 0.5, 0.5}, {0.75, 0.5, 0.25});
  success &= check({210, 0.5, 0.4}, {0.2, 0.4, 0.6});
  success &= check({240, 1, 0.75}, {0.5, 0.5, 1});

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
