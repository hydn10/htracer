#ifndef HTRACER_BENCHMARKS_SCENE_SPEC_HPP
#define HTRACER_BENCHMARKS_SCENE_SPEC_HPP


#include <cstdint>
#include <variant>


namespace htracer::benchmarks
{

class geometry_count
{
public:
  [[nodiscard]]
  static geometry_count
  make(std::uint32_t value);

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept
  {
    return value_;
  }

private:
  explicit constexpr geometry_count(std::uint32_t value) noexcept
      : value_{value}
  {
  }

  std::uint32_t value_;
};


struct mixed_scene
{
};


struct traversal_scene
{
  geometry_count count;
};


struct rng_probe_scene
{
};


using scene_spec = std::variant<mixed_scene, traversal_scene, rng_probe_scene>;
using deterministic_scene_spec = std::variant<mixed_scene, traversal_scene>;

} // namespace htracer::benchmarks

#endif
