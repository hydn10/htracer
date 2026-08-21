#ifndef HTRACER_BENCHMARKS_SCENE_SPEC_HPP
#define HTRACER_BENCHMARKS_SCENE_SPEC_HPP


#include <cstdint>
#include <expected>
#include <string_view>
#include <variant>


namespace htracer::benchmarks
{

class geometry_count
{
  std::uint32_t value_;

  explicit constexpr geometry_count(std::uint32_t value) noexcept
      : value_{value}
  {
  }

public:
  [[nodiscard]]
  static constexpr std::expected<geometry_count, std::string_view>
  try_make(std::uint32_t value) noexcept;

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept
  {
    return value_;
  }
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


constexpr std::expected<geometry_count, std::string_view>
geometry_count::try_make(std::uint32_t value) noexcept
{
  if (value == 0)
  {
    return std::unexpected{"geometry count must be greater than zero"};
  }

  return geometry_count{value};
}

} // namespace htracer::benchmarks

#endif
