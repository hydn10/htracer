#include <htracer_benchmarks/cli.hpp>
#include <htracer_benchmarks/model.hpp>
#include <htracer_benchmarks/render_benchmark.hpp>
#include <htracer_benchmarks/report.hpp>
#include <htracer_benchmarks/suite.hpp>

#include <array>
#include <concepts>
#include <cstddef>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <new>
#include <optional>
#include <print>
#include <span>
#include <utility>
#include <variant>


namespace
{

void
run_cases(
    std::span<htracer::benchmarks::benchmark_case const> cases, std::optional<std::filesystem::path> const &output)
{
  using namespace htracer::benchmarks;

  run_report report{.environment = get_environment_info(), .results = {}};
  report.results.reserve(cases.size());
  print_environment(report.environment);

  for (auto const &benchmark : cases)
  {
    std::println("Running {}...", benchmark_name(benchmark));
    (void)std::fflush(stdout);
    auto result = run_benchmark(benchmark);
    print_result(result);
    report.results.push_back(std::move(result));
  }

  if (output)
  {
    write_json(*output, report);
    std::println("Wrote {}", output->string());
  }
}


int
run(std::span<char const *const> arguments)
{
  using namespace htracer::benchmarks;

  return std::visit(
      []<typename Command>(Command const &command)
  {
    if constexpr (std::same_as<Command, help_command>)
    {
      print_help();
    }
    else if constexpr (std::same_as<Command, list_command>)
    {
      print_case_list(quick_suite_catalog::cases());
    }
    else if constexpr (std::same_as<Command, quick_suite_command>)
    {
      run_cases(quick_suite_catalog::cases(), command.output);
    }
    else
    {
      static_assert(std::same_as<Command, custom_render_command>);
      std::array benchmarks{benchmark_case::custom(command.benchmark)};
      run_cases(benchmarks, command.output);
    }
    return 0;
  },
      parse_cli(arguments));
}

} // namespace


int
main(int argc, char const *argv[])
{
  try
  {
    return run(
        {argv + 1, static_cast<std::size_t>(argc - 1)}); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  }
  catch (htracer::benchmarks::usage_error const &error)
  {
    (void)std::fputs("error: ", stderr);
    (void)std::fputs(error.what(), stderr);
    (void)std::fputs("\nTry --help for usage.\n", stderr);
    return 2;
  }
  catch (std::bad_alloc const &)
  {
    (void)std::fputs("error: benchmark allocation failed\n", stderr);
    return 1;
  }
  catch (std::exception const &error)
  {
    (void)std::fputs("error: ", stderr);
    (void)std::fputs(error.what(), stderr);
    (void)std::fputc('\n', stderr);
    return 1;
  }
  catch (...)
  {
    (void)std::fputs("error: benchmark failed with an unknown exception\n", stderr);
    return 1;
  }
}
