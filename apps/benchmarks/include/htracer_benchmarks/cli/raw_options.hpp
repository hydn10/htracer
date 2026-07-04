#ifndef HTRACER_BENCHMARKS_CLI_RAW_OPTIONS_HPP
#define HTRACER_BENCHMARKS_CLI_RAW_OPTIONS_HPP


#include <htracer_benchmarks/cli.hpp>

#include <charconv>
#include <concepts>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>


namespace htracer::benchmarks::cli
{

struct raw_options
{
  bool help{};
  bool list{};
  std::optional<std::string> suite;
  std::optional<std::string> benchmark;
  std::optional<std::string> scene;
  std::optional<std::string> rendering;
  std::optional<std::string> width;
  std::optional<std::string> height;
  std::optional<std::string> geometry_count;
  std::optional<std::string> samples;
  std::optional<std::string> precision;
  std::optional<std::string> policy;
  std::optional<std::string> seed;
  std::optional<std::string> warmup;
  std::optional<std::string> repetitions;
  std::optional<std::string> output;
};


[[nodiscard]]
raw_options
parse_raw_options(std::span<char const *const> arguments);


void
require(bool condition, std::string const &message);


template<typename UInt>
requires std::is_unsigned_v<UInt>
[[nodiscard]]
UInt
parse_unsigned(std::string const &text, std::string_view option)
{
  UInt value{};
  auto const *const begin = text.data();
  auto const *const finish = begin + text.size(); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  auto const [end, error] = std::from_chars(begin, finish, value);
  if (error != std::errc{} || end != finish)
  {
    throw usage_error("invalid unsigned integer for " + std::string{option} + ": " + text);
  }
  return value;
}


[[nodiscard]]
bool
has_custom_options(raw_options const &options);

} // namespace htracer::benchmarks::cli

#endif
