#ifndef HTRACER_OUTPUT_PPM_HPP
#define HTRACER_OUTPUT_PPM_HPP


#include <htracer/raytracing/image.hpp>


namespace htracer::output
{
template<typename Ostream>
class ppm
{
  Ostream& out_;

public:
  explicit ppm(Ostream& ostream);

  template<typename Float>
  ppm<Ostream>&
  operator<<(const raytracing::image<Float>& image);
};


template<typename Ostream>
ppm<Ostream>::ppm(Ostream& ostream) : out_(ostream)
{
}


template<typename Ostream>
template<typename Float>
ppm<Ostream>&
ppm<Ostream>::operator<<(const raytracing::image<Float>& image)
{
  out_ << "P6\n" << image.h_res() << " " << image.v_res() << "\n255\n";

  for (const auto& pixel: image.pixels())
  {
    constexpr auto BYTE = 255;

    const auto final_pixel = pixel.to_srgb();

    out_ << static_cast<unsigned char>(std::round(BYTE * final_pixel.r()))
         << static_cast<unsigned char>(std::round(BYTE * final_pixel.g()))
         << static_cast<unsigned char>(std::round(BYTE * final_pixel.b()));
  }

  return *this;
}

} // namespace htracer::output

#endif // HTRACER_OUTPUT_PPM_HPP
