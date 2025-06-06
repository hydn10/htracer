#ifndef HTRACER_GEOMETRIES_SPHERE_HPP
#define HTRACER_GEOMETRIES_SPHERE_HPP


#include <htracer/geometries/ray.hpp>
#include <htracer/vector.hpp>

#include <optional>
#include <utility>


namespace htracer::geometries
{

template<typename Float>
class sphere final : public geometry<Float>
{
  v3<Float> center_;
  Float radius_;

public:
  constexpr sphere(v3<Float> const &center, Float radius);

  [[nodiscard]]
  constexpr v3<Float> const &
  center() const;

  [[nodiscard]]
  constexpr Float
  radius() const;

  // TODO: nodiscard?
  // TODO: Return 0 as distance if no intersection and check performance?
  //       Should be careful as in theory it can return negative values.
  constexpr std::optional<Float>
  intersect(ray<Float> const &ray) const;

  // TODO: nodiscard? how does it work with inheritance?
  constexpr v3<Float>
  normal(v3<Float> const &point) const override;
};


template<typename Float>
constexpr sphere<Float>::sphere(v3<Float> const &center, Float radius)
    : center_{center}
    , radius_{radius}
{
}


template<typename Float>
constexpr v3<Float> const &
sphere<Float>::center() const
{
  return center_;
}


template<typename Float>
constexpr Float
sphere<Float>::radius() const
{
  return radius_;
}


template<typename Float>
constexpr std::optional<Float>
sphere<Float>::intersect(ray<Float> const &ray) const
{
  auto oc = center_ - ray.origin;

  auto dot_voc = dot(ray.direction, oc);
  auto dot_ococ = dot(oc, oc);

  auto disc = dot_voc * dot_voc - dot_ococ + radius_ * radius_;

  if (disc <= 0)
  {
    return {};
  }

  return dot_voc - std::sqrt(disc);
}


template<typename Float>
constexpr v3<Float>
sphere<Float>::normal(v3<Float> const &point) const
{
  return normalize(point - center_);
}

} // namespace htracer::geometries

#endif
