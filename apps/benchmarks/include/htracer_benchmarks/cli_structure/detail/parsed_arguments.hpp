#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_PARSED_ARGUMENTS_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_PARSED_ARGUMENTS_HPP


#include <htracer_benchmarks/cli_structure/detail/option_descriptor.hpp>

#include <string_view>
#include <vector>


namespace htracer::benchmarks::cli_structure::detail
{

class parsed_arguments
{
  std::vector<option_occurrence> values_;

public:
  explicit parsed_arguments(std::vector<option_occurrence> values);

  [[nodiscard]]
  option_occurrence const *
  find(std::string_view name) const noexcept;
};

} // namespace htracer::benchmarks::cli_structure::detail

#endif
