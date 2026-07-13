#include <htracer_benchmarks/reporting/environment.hpp>

#include <htracer_benchmarks/run_report.hpp>

#include <string>
#include <thread>
#include <utility>


namespace htracer::benchmarks::reporting
{

environment_info
get_environment_info()
{
#ifdef _WIN32
  std::string operating_system = "windows";
#elifdef __linux__
  std::string operating_system = "linux";
#else
  std::string operating_system = "unknown";
#endif

#if defined(_M_X64) || defined(__x86_64__)
  std::string architecture = "x86_64";
#elif defined(_M_ARM64) || defined(__aarch64__)
  std::string architecture = "arm64";
#else
  std::string architecture = "unknown";
#endif

#ifdef __clang__
  std::string compiler = "clang";
  std::string compiler_version = std::to_string(__clang_major__) + "." + std::to_string(__clang_minor__) + "." +
                                 std::to_string(__clang_patchlevel__);
#elifdef _MSC_VER
  std::string compiler = "msvc";
  std::string compiler_version = std::to_string(_MSC_FULL_VER);
#elifdef __GNUC__
  std::string compiler = "gcc";
  std::string compiler_version =
      std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__) + "." + std::to_string(__GNUC_PATCHLEVEL__);
#else
  std::string compiler = "unknown";
  std::string compiler_version = "unknown";
#endif

  return {
      .os = std::move(operating_system),
      .architecture = std::move(architecture),
      .compiler = std::move(compiler),
      .compiler_version = std::move(compiler_version),
      .build_type = HTRACER_BENCHMARK_BUILD_TYPE,
      .logical_processors = std::thread::hardware_concurrency()};
}

} // namespace htracer::benchmarks::reporting
