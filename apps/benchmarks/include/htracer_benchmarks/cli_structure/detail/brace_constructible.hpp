#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_BRACE_CONSTRUCTIBLE_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_BRACE_CONSTRUCTIBLE_HPP


#include <utility>


namespace htracer::benchmarks::cli_structure::detail
{

template<typename Result, typename... Arguments>
concept brace_constructible_from =
    requires(Arguments &&...arguments) { Result{std::forward<Arguments>(arguments)...}; };

} // namespace htracer::benchmarks::cli_structure::detail

#endif
