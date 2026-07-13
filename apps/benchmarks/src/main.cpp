#include <htracer_benchmarks/app.hpp>
#include <htracer_benchmarks/cli.hpp>
#include <htracer_benchmarks/exit.hpp>

#include <cstddef>
#include <exception>
#include <iostream>
#include <new>
#include <ostream> // IWYU pragma: keep
#include <print>   // IWYU pragma: keep
#include <span>


namespace
{

[[nodiscard]]
htracer::benchmarks::diagnostic_exit
run_with_diagnostics(std::span<char const *const> arguments)
try
{
  htracer::benchmarks::run_app(arguments);
  return htracer::benchmarks::diagnostic_exit::success();
}
catch (htracer::benchmarks::usage_error const &error)
{
  std::println(std::cerr, "error: {}\nTry --help for usage.", error.what());
  return htracer::benchmarks::diagnostic_exit::usage_error();
}
catch (std::bad_alloc const &)
{
  std::println(std::cerr, "error: benchmark allocation failed");
  return htracer::benchmarks::diagnostic_exit::failure();
}
catch (std::exception const &error)
{
  std::println(std::cerr, "error: {}", error.what());
  return htracer::benchmarks::diagnostic_exit::failure();
}
catch (...)
{
  std::println(std::cerr, "error: benchmark failed with an unknown exception");
  return htracer::benchmarks::diagnostic_exit::failure();
}


[[nodiscard]]
htracer::benchmarks::process_exit
run_noexcept(std::span<char const *const> arguments) noexcept
try
{
  return htracer::benchmarks::process_exit{run_with_diagnostics(arguments)};
}
catch (...)
{
  return htracer::benchmarks::process_exit{htracer::benchmarks::diagnostic_exit::failure()};
}

} // namespace


int
main(int argc, char const *argv[])
{
  auto const arguments = std::span{
      argv + 1, static_cast<std::size_t>(argc - 1)}; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  return run_noexcept(arguments).to_int();
}
