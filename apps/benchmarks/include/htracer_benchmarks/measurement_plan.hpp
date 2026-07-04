#ifndef HTRACER_BENCHMARKS_MEASUREMENT_PLAN_HPP
#define HTRACER_BENCHMARKS_MEASUREMENT_PLAN_HPP


#include <cstdint>


namespace htracer::benchmarks
{

class repetition_count
{
public:
  [[nodiscard]]
  static repetition_count
  make(std::uint32_t value);

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept
  {
    return value_;
  }

private:
  explicit constexpr repetition_count(std::uint32_t value) noexcept
      : value_{value}
  {
  }

  std::uint32_t value_;
};


class warmup_count
{
public:
  explicit constexpr warmup_count(std::uint32_t value) noexcept
      : value_{value}
  {
  }

  [[nodiscard]]
  constexpr std::uint32_t
  value() const noexcept
  {
    return value_;
  }

private:
  std::uint32_t value_;
};


struct measurement_plan
{
  warmup_count warmups;
  repetition_count repetitions;
};

} // namespace htracer::benchmarks

#endif
