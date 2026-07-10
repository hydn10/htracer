#include <htracer_benchmarks/exit.hpp>


namespace htracer::benchmarks
{

diagnostic_exit::diagnostic_exit(exit_code code) noexcept
    : code_{code}
{
}


diagnostic_exit
diagnostic_exit::success() noexcept
{
  return diagnostic_exit{exit_code::success};
}


diagnostic_exit
diagnostic_exit::failure() noexcept
{
  return diagnostic_exit{exit_code::failure};
}


diagnostic_exit
diagnostic_exit::usage_error() noexcept
{
  return diagnostic_exit{exit_code::usage_error};
}


process_exit::process_exit(diagnostic_exit exit) noexcept
    : code_{exit.code_}
{
}


int
process_exit::to_int() const noexcept
{
  return static_cast<int>(code_);
}

} // namespace htracer::benchmarks
