#ifndef HTRACER_BENCHMARKS_EXECUTION_CHECKSUM_ACCUMULATOR_HPP
#define HTRACER_BENCHMARKS_EXECUTION_CHECKSUM_ACCUMULATOR_HPP


#include <cstddef>
#include <cstdint>
#include <span>
#include <type_traits>


namespace htracer::benchmarks::execution
{

class checksum_accumulator
{
  static constexpr std::uint64_t fnv_offset_basis{14695981039346656037ULL};
  static constexpr std::uint64_t fnv_prime{1099511628211ULL};

  std::uint64_t value_{fnv_offset_basis};

public:
  template<typename T>
  requires std::is_trivially_copyable_v<T>
  constexpr void
  append(T const &value) noexcept
  {
    append(std::as_bytes(std::span{&value, std::size_t{1}}));
  }

  constexpr void
  append(std::span<std::byte const> bytes) noexcept
  {
    for (auto const byte : bytes)
    {
      value_ ^= std::to_integer<std::uint8_t>(byte);
      value_ *= fnv_prime;
    }
  }

  [[nodiscard]]
  constexpr std::uint64_t
  value() const noexcept
  {
    return value_;
  }
};

} // namespace htracer::benchmarks::execution

#endif
