#include <htracer_benchmarks/cli.hpp>
#include <htracer_benchmarks/model.hpp>
#include <htracer_benchmarks/render_benchmark.hpp>
#include <htracer_benchmarks/report.hpp>
#include <htracer_benchmarks/suite.hpp>

#include <cstddef>
#include <exception>
#include <iostream>
#include <new>
#include <span>
#include <utility>
#include <vector>


namespace
{

int
run(std::span<char const *const> arguments)
{
  using namespace htracer::benchmarks;

  auto const invocation = parse_cli(arguments);
  if (invocation.operation == operation_kind::help)
  {
    print_help(std::cout);
    return 0;
  }

  auto cases = make_quick_suite();
  if (invocation.operation == operation_kind::list)
  {
    print_case_list(std::cout, cases);
    return 0;
  }

  if (invocation.operation == operation_kind::custom_render)
  {
    cases.clear();
    cases.push_back(*invocation.custom_case);
  }

  run_report report{.environment = get_environment_info(), .results = {}};
  report.results.reserve(cases.size());
  print_environment(std::cout, report.environment);

  for (auto const &benchmark : cases)
  {
    std::cout << "Running " << benchmark.id << "...\n" << std::flush;
    auto result = run_benchmark(benchmark);
    print_result(std::cout, result);
    report.results.push_back(std::move(result));
  }

  if (invocation.output_path)
  {
    write_json(*invocation.output_path, report);
    std::cout << "Wrote " << invocation.output_path->string() << '\n';
  }

  return 0;
}

} // namespace


int
main(int argc, char const *argv[]) // NOLINT(bugprone-exception-escape)
{
  try
  {
    return run(
        {argv + 1, static_cast<std::size_t>(argc - 1)}); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  }
  catch (htracer::benchmarks::usage_error const &error)
  {
    std::cerr << "error: " << error.what() << "\nTry --help for usage.\n";
    return 2;
  }
  catch (std::bad_alloc const &)
  {
    std::cerr << "error: benchmark allocation failed\n";
    return 1;
  }
  catch (std::exception const &error)
  {
    std::cerr << "error: " << error.what() << '\n';
    return 1;
  }
}
