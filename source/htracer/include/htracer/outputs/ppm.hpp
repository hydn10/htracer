#ifndef HTRACER_OUTPUT_PPM_HPP
#define HTRACER_OUTPUT_PPM_HPP


#include <htracer/raytracing/image.hpp>

#include <string>


namespace htracer::outputs
{

template<typename Ostream>
class ppm
{
  Ostream &out_;

public:
  explicit ppm(Ostream &ostream);

  template<typename Float>
  ppm<Ostream> &
  operator<<(raytracing::image<Float> const &image);
};


template<typename Ostream>
ppm<Ostream>::ppm(Ostream &ostream)
    : out_(ostream)
{
}


template<typename Ostream>
template<typename Float>
ppm<Ostream> &
ppm<Ostream>::operator<<(raytracing::image<Float> const &image)
{
  constexpr auto NUM_COLORS = 256;

  out_ << "P6\n" << image.h_res() << " " << image.v_res() << '\n' << std::to_string(NUM_COLORS - 1) << '\n';

  // TODO: Implement with ranges?
  for (auto const &pixel : image.pixels())
  {
    auto const to_out_value = [](Float x) -> std::uint8_t
    {
      auto const scaled = NUM_COLORS * x;

      // Handle case where x == 1.0.
      // Another option is scaling by (NUM_COLORS - .000001), but I feel it is more correct this way.
      // The branch predictor should almost always take the other path anyway.
      if (scaled >= NUM_COLORS)
        return NUM_COLORS - 1;

      return scaled;
    };

    const auto final_pixel = pixel.to_srgb();

    out_ << to_out_value(final_pixel.r()) << to_out_value(final_pixel.g()) << to_out_value(final_pixel.b());
  }

  return *this;
}

} // namespace htracer::outputs

#endif