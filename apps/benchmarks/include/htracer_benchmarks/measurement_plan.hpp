#ifndef HTRACER_BENCHMARKS_MEASUREMENT_PLAN_HPP
#define HTRACER_BENCHMARKS_MEASUREMENT_PLAN_HPP


#include <cstdint>
#include <expected>
#include <string_view>


namespace htracer::benchmarks
{

class repetition_count
{
  std::uint32_t value_;

  explicit constexpr repetition_count(std::uint32_t value) noexcept;

public:
  [[nodiscard]]
  static constexpr std::expected<repetition_count, std::string_view>
  try_make(std::uint32_t value) noexcept;

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept;
};


class warmup_count
{
  std::uint32_t value_;

public:
  explicit constexpr warmup_count(std::uint32_t value) noexcept;

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept;
};


struct measurement_plan
{
  warmup_count warmups;
  repetition_count repetitions;
};


constexpr repetition_count::repetition_count(std::uint32_t value) noexcept
    : value_{value}
{
}


constexpr std::uint32_t
repetition_count::value() const noexcept
{
  return value_;
}


constexpr warmup_count::warmup_count(std::uint32_t value) noexcept
    : value_{value}
{
}


constexpr std::uint32_t
warmup_count::value() const noexcept
{
  return value_;
}


constexpr std::expected<repetition_count, std::string_view>
repetition_count::try_make(std::uint32_t value) noexcept
{
  if (value == 0)
  {
    return std::unexpected{"repetition count must be greater than zero"};
  }
  
  return repetition_count{value};
}

} // namespace htracer::benchmarks

#endif
