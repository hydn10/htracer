#ifndef HTRACER_BENCHMARKS_EXIT_HPP
#define HTRACER_BENCHMARKS_EXIT_HPP


#include <cstdint>


namespace htracer::benchmarks
{

enum class exit_code : std::uint8_t
{
  success = 0,
  failure = 1,
  usage_error = 2,
};


class diagnostic_exit
{
  friend class process_exit;

  explicit diagnostic_exit(exit_code code) noexcept;

  exit_code code_;

public:
  [[nodiscard]]
  static diagnostic_exit
  success() noexcept;

  [[nodiscard]]
  static diagnostic_exit
  failure() noexcept;

  [[nodiscard]]
  static diagnostic_exit
  usage_error() noexcept;
};


class process_exit
{
  exit_code code_;

public:
  explicit process_exit(diagnostic_exit exit) noexcept;

  [[nodiscard]]
  int
  to_int() const noexcept;
};

} // namespace htracer::benchmarks

#endif
