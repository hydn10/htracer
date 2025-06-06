#ifndef HTRACER_RENDERING_CAMERA_HPP
#define HTRACER_RENDERING_CAMERA_HPP


#include <htracer/vector.hpp>

#include <concepts>


namespace htracer::rendering
{

template<std::floating_point Float>
class camera
{
  v3<Float> position_;

  v3<Float> view_;
  v3<Float> up_;
  v3<Float> right_;

  uint32_t h_res_;
  uint32_t v_res_;
  Float fov_;

public:
  constexpr camera(
      v3<Float> const &position,
      v3<Float> const &view,
      v3<Float> const &up,
      uint32_t horizontal_resolution,
      uint32_t vertical_resolution,
      Float fov);

  constexpr v3<Float> const &
  position() const noexcept;
  constexpr v3<Float> const &
  view() const noexcept;
  constexpr v3<Float> const &
  up() const noexcept;
  constexpr v3<Float> const &
  right() const noexcept;
  constexpr uint32_t
  h_res() const noexcept;
  constexpr uint32_t
  v_res() const noexcept;
  constexpr Float
  fov() const noexcept;
};


template<std::floating_point Float>
constexpr camera<Float>::camera(
    v3<Float> const &position,
    v3<Float> const &view,
    v3<Float> const &up,
    uint32_t horizontal_resolution,
    uint32_t vertical_resolution,
    Float fov)
    : position_{position}
    , h_res_{horizontal_resolution}
    , v_res_{vertical_resolution}
    , fov_{fov}
{
  view_ = normalize(view);
  right_ = normalize(cross(view, up));
  up_ = cross(right_, view_);
}


template<std::floating_point Float>
constexpr v3<Float> const &
camera<Float>::position() const noexcept
{
  return position_;
}


template<std::floating_point Float>
constexpr v3<Float> const &
camera<Float>::view() const noexcept
{
  return view_;
}


template<std::floating_point Float>
constexpr v3<Float> const &
camera<Float>::up() const noexcept
{
  return up_;
}


template<std::floating_point Float>
constexpr v3<Float> const &
camera<Float>::right() const noexcept
{
  return right_;
}


template<std::floating_point Float>
constexpr uint32_t
camera<Float>::h_res() const noexcept
{
  return h_res_;
}


template<std::floating_point Float>
constexpr uint32_t
camera<Float>::v_res() const noexcept
{
  return v_res_;
}


template<std::floating_point Float>
constexpr Float
camera<Float>::fov() const noexcept
{
  return fov_;
}

} // namespace htracer::rendering

#endif
