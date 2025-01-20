#ifndef HTRACER_OUTPUTS_PPM_HPP
#define HTRACER_OUTPUTS_PPM_HPP


#include <htracer/rendering/image.hpp>

#include <bit>
#include <fstream>
#include <string>


namespace htracer::outputs
{


class ppm
{
public:
  enum class bytes_per_value
  {
    BPV1,
    BPV2,
  };

  template<bytes_per_value BPV, typename Float>
  void
  save(std::string_view filename, rendering::image<Float> const &image) const;
};


namespace detail_
{

template<ppm::bytes_per_value BPV>
struct bpv_traits
{
};


template<>
struct bpv_traits<ppm::bytes_per_value::BPV1>
{
  static auto constexpr NUM_COLORS = 256u;
  using pixel_value_t = std::uint8_t;
};


template<>
struct bpv_traits<ppm::bytes_per_value::BPV2>
{
  static auto constexpr NUM_COLORS = 65536u;
  using pixel_value_t = std::uint16_t;

  static pixel_value_t
  to_big_endian(pixel_value_t value)
  {
    return (value >> 8) | (value << 8);
  }
};

} // namespace detail_


template<ppm::bytes_per_value BPV, typename Float>
void
ppm::save(std::string_view filename, rendering::image<Float> const &image) const
{
  // http://netpbm.sourceforge.net/doc/ppm.html

  using traits_t = detail_::bpv_traits<BPV>;

  auto constexpr NUM_COLORS = traits_t::NUM_COLORS;
  using pixel_value_t = typename traits_t::pixel_value_t;

  auto const to_pixel_value = [](Float x) -> pixel_value_t
  {
    auto scaled = NUM_COLORS * x;

    // Handle case where x == 1.0.
    // Another option is scaling by (NUM_COLORS - .000001), but I feel it is more correct this way.
    // The branch predictor should almost always take the other path anyway.
    if (scaled >= NUM_COLORS) [[unlikely]]
    {
      scaled = NUM_COLORS - 1;
    }

    auto const res = static_cast<pixel_value_t>(scaled);

    if constexpr (BPV != ppm::bytes_per_value::BPV1 && std::endian::native != std::endian::big)
    {
      return traits_t::to_big_endian(res);
    }
    else
    {
      return res;
    }
  };

  auto const write_value = [](auto &stream, pixel_value_t value)
  {
    stream.write(reinterpret_cast<char const *>(&value), sizeof value);
  };

  std::ofstream out(filename.data(), std::ios::out | std::ios::binary);

  out << "P6\n" << image.h_res() << " " << image.v_res() << '\n' << std::to_string(NUM_COLORS - 1) << '\n';

  for (auto const &pixel : image.pixels())
  {
    auto const final_pixel = pixel.to_srgb();

    write_value(out, to_pixel_value(final_pixel.r()));
    write_value(out, to_pixel_value(final_pixel.g()));
    write_value(out, to_pixel_value(final_pixel.b()));
  }
}

} // namespace htracer::outputs

#endif
