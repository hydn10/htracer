#ifndef HTRACER_GEOMETRY_PLANE_HPP
#define HTRACER_GEOMETRY_PLANE_HPP


#include <htracer/geometries/geometry.hpp>
#include <htracer/geometries/ray.hpp>
#include <htracer/vector.hpp>

#include <optional>


namespace htracer::geometries
{

template<typename Float>
class plane final : public geometry<Float>
{
  v3<Float> anchor_;
  v3<Float> up_;

public:
  constexpr plane(v3<Float> const &anchor, v3<Float> const &up) noexcept;

  [[nodiscard]]
  constexpr v3<Float> const &
  anchor() const noexcept;

  [[nodiscard]]
  constexpr v3<Float> const &
  up() const noexcept;

  [[nodiscard]]
  constexpr std::optional<Float>
  intersect(ray<Float> const &ray) const noexcept;

  // TODO: nodiscard? how does it work with inheritance?
  constexpr v3<Float>
  normal(v3<Float> const &point) const noexcept override;
};


template<typename Float>
constexpr plane<Float>::plane(v3<Float> const &anchor, v3<Float> const &up) noexcept
    : anchor_{anchor}
    , up_{normalize(up)}
{
}


template<typename Float>
constexpr v3<Float> const &
plane<Float>::anchor() const noexcept
{
  return anchor_;
}


template<typename Float>
constexpr v3<Float> const &
plane<Float>::up() const noexcept
{
  return up_;
}


template<typename Float>
constexpr std::optional<Float>
plane<Float>::intersect(ray<Float> const &ray) const noexcept
{
  auto dot_dn = dot(ray.direction, up_);

  if (dot_dn == Float{0})
  {
    return {};
  }

  auto dot_aon = dot(anchor_ - ray.origin, up_);
  return dot_aon / dot_dn;
}


template<typename Float>
constexpr v3<Float>
plane<Float>::normal(v3<Float> const &) const noexcept
{
  return up_;
}

} // namespace htracer::geometries

#endif
