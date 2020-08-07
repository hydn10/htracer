#ifndef HTRACER_RAYTRACING_CAMERA_HPP
#define HTRACER_RAYTRACING_CAMERA_HPP


#include <htracer/raytracing/sampler.hpp>
#include <htracer/scene/scene.hpp>
#include <htracer/vector.hpp>

#include <cmath>
#include <ostream>
#include <vector>


namespace htracer::raytracing
{
template<typename Float>
class camera
{
  v3<Float> origin_;

  v3<Float> view_;
  v3<Float> up_;
  v3<Float> right_;

  unsigned h_res_;
  unsigned v_res_;
  Float fov_;

public:
  camera(
      v3<Float> origin,
      const v3<Float>& view,
      const v3<Float>& up,
      unsigned horizontal_resolution,
      unsigned vertical_resolution,
      Float fov);

  void
  render(std::ostream& out, const scene::scene<Float>& scene) const;
};


template<typename Float>
camera<Float>::camera(
    v3<Float> origin,
    const v3<Float>& view,
    const v3<Float>& up,
    unsigned horizontal_resolution,
    unsigned vertical_resolution,
    Float fov)
    : origin_{origin}
    , h_res_{horizontal_resolution}
    , v_res_{vertical_resolution}
    , fov_{fov}
{
  view_ = view.normalized();
  right_ = cross(view, up).normalized();
  up_ = cross(right_, view_);
}


template<typename Float>
void
camera<Float>::render(std::ostream& out, const scene::scene<Float>& scene) const
{
  out << "P6\n" << h_res_ << " " << v_res_ << "\n255\n";

  auto h_tan = std::tan(fov_);
  auto v_tan = h_tan * v_res_ / h_res_;

  for (auto i = 0u; i < v_res_; ++i)
  {
    auto dv = v_tan * (1 - (Float(2 * i) / (v_res_ - 1)));

    for (auto j = 0u; j < h_res_; ++j)
    {
      auto dh = h_tan * ((Float(2 * j) / (h_res_ - 1)) - 1);

      auto dir = view_ + dv * up_ + dh * right_;
      dir = dir.normalized();

      auto pixel = sample({origin_, dir}, scene);

      out << (unsigned char)std::round(255 * pixel[0])
          << (unsigned char)std::round(255 * pixel[1])
          << (unsigned char)std::round(255 * pixel[2]);
    }
  }
}

} // namespace htracer::raytracing

#endif // HTRACER_RAYTRACING_CAMERA_HPP
