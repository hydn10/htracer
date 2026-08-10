#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_COMMANDS_APPLICATION_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_COMMANDS_APPLICATION_HPP


#include <htracer_benchmarks/cli_structure/detail/argument_view.hpp>
#include <htracer_benchmarks/cli_structure/detail/type_list.hpp>

#include <span>
#include <string>
#include <utility>


namespace htracer::benchmarks::cli_structure
{

template<typename Root>
class application
{
  Root root_;

public:
  using invocation_type = typename detail::invocation_from_results<typename Root::result_types>::type;

  constexpr explicit application(Root root);

  [[nodiscard]]
  invocation_type
  parse(std::span<char const *const> arguments) const;
};


template<typename Root>
[[nodiscard]]
constexpr auto
make_application(Root root);


template<typename Root>
using invocation_t = typename application<Root>::invocation_type;


template<typename Root>
constexpr application<Root>::application(Root root)
    : root_{std::move(root)}
{
}


template<typename Root>
typename application<Root>::invocation_type
application<Root>::parse(std::span<char const *const> arguments) const
{
  return root_.template parse<invocation_type>(detail::argument_view{arguments}, std::string{root_.name()});
}


template<typename Root>
constexpr auto
make_application(Root root)
{
  return application<Root>{std::move(root)};
}

} // namespace htracer::benchmarks::cli_structure

#endif
