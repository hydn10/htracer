#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_TYPE_LIST_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_TYPE_LIST_HPP


#include <htracer_benchmarks/cli_structure/foundations/help_page.hpp>

#include <variant>


namespace htracer::benchmarks::cli_structure::detail
{

template<typename... Types>
struct type_list
{
};


template<typename Left, typename Right>
struct concatenate_two;


template<typename... Left, typename... Right>
struct concatenate_two<type_list<Left...>, type_list<Right...>>
{
  using type = type_list<Left..., Right...>;
};


template<typename... Lists>
struct concatenate;


template<>
struct concatenate<>
{
  using type = type_list<>;
};


template<typename First, typename... Remaining>
struct concatenate<First, Remaining...>
{
  using type = typename concatenate_two<First, typename concatenate<Remaining...>::type>::type;
};


template<typename Results>
struct invocation_from_results;


template<typename... Results>
struct invocation_from_results<type_list<Results...>>
{
  using type = std::variant<help_page, Results...>;
};

} // namespace htracer::benchmarks::cli_structure::detail

#endif
