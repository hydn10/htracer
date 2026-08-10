#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_ARGUMENT_VIEW_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_ARGUMENT_VIEW_HPP


#include <cstddef>
#include <span>
#include <string_view>


namespace htracer::benchmarks::cli_structure::detail
{

class argument_view
{
  std::span<char const *const> values_;

public:
  explicit argument_view(std::span<char const *const> values) noexcept;

  [[nodiscard]]
  bool
  empty() const noexcept;

  [[nodiscard]]
  std::size_t
  size() const noexcept;

  [[nodiscard]]
  std::string_view
  operator[](std::size_t index) const noexcept;

  [[nodiscard]]
  argument_view
  subspan(std::size_t offset) const noexcept;

  [[nodiscard]]
  bool
  contains_help() const noexcept;
};

} // namespace htracer::benchmarks::cli_structure::detail

#endif
