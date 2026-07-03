#ifndef HTRACER_BENCHMARKS_CLI_HPP
#define HTRACER_BENCHMARKS_CLI_HPP


#include <htracer_benchmarks/model.hpp>

#include <span>
#include <stdexcept>


namespace htracer::benchmarks
{

class usage_error : public std::runtime_error
{
public:
  using std::runtime_error::runtime_error;
};


[[nodiscard]]
invocation
parse_cli(std::span<char const *const> arguments);


void
print_help();

} // namespace htracer::benchmarks

#endif
