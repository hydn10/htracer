#ifndef HDN_RAYTRACER_CAMERA_HPP
#define HDN_RAYTRACER_CAMERA_HPP


#include <raytracer/vector.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/sampler.hpp>

#include <ostream>
#include <vector>
#include <cmath>


namespace hdn
{

template <typename T>
class camera
{
  v3<T> origin_;

  v3<T> view_;
  v3<T> up_;
  v3<T> right_;

  unsigned h_res_;
  unsigned v_res_;
  T fov_;

public:
  camera(v3<T> origin, const v3<T> &view, const v3<T> &up,
      unsigned horizontal_resolution, unsigned vertical_resolution, T fov);
  ~camera() = default;

  void
  render(std::ostream &out, const std::vector<hdn::sphere<T>> &objects) const;
};



template <typename T>
camera<T>::camera(v3<T> origin, const v3<T> &view, const v3<T> &up,
      unsigned horizontal_resolution, unsigned vertical_resolution, T fov)
  :
    origin_{origin},
    h_res_{horizontal_resolution},
    v_res_{vertical_resolution},
    fov_{fov * std::atan(1) / 45}
{
  view_ = view.normalized();
  right_ = cross(view, up).normalized();
  up_ = cross(right_, view_);
}


template <typename T>
void
camera<T>::render(std::ostream &out, const std::vector<hdn::sphere<T>> &objects) const
{
  out << "P6\n" << h_res_ << " " << v_res_ << "\n255\n"; 

  auto h_tan = std::tan(fov_);
  auto v_tan = h_tan * v_res_ / h_res_;

  for (auto i = 0u; i < v_res_; ++i)
  {
    auto dv = v_tan * (1 - (T(2 * i) / (v_res_ - 1)));

    for (auto j = 0u; j < h_res_; ++j)
    {
      auto dh = h_tan * ((T(2 * j) / (h_res_ - 1)) - 1);

      auto dir = view_ + dv*up_ + dh*right_;
      dir = dir.normalized();

      auto pixel = sample({origin_, dir}, objects);

      out << (unsigned char)pixel.r << (unsigned char)pixel.g << (unsigned char) pixel.b;
    }
  }
}

}

#endif // HDN_RAYTRACER_CAMERA_HPP