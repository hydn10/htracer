#include <htracer_benchmarks/cli/raw_options.hpp>

#include <htracer_benchmarks/cli.hpp>

#include <cstddef>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>


namespace htracer::benchmarks::cli
{

namespace
{

template<typename T>
void
set_once(std::optional<T> &destination, T value, std::string_view option)
{
  if (destination)
  {
    throw usage_error("duplicate option: " + std::string{option});
  }
  destination = std::move(value);
}


void
set_flag_once(bool &destination, std::string_view option)
{
  if (destination)
  {
    throw usage_error("duplicate option: " + std::string{option});
  }
  destination = true;
}


[[nodiscard]]
std::string
take_value(std::size_t &index, std::span<char const *const> arguments, std::string_view option)
{
  if (index + 1 >= arguments.size())
  {
    throw usage_error("missing value for option: " + std::string{option});
  }
  std::string value{arguments[++index]};
  if (value.starts_with("--"))
  {
    throw usage_error("missing value for option: " + std::string{option});
  }
  return value;
}

} // namespace


raw_options
parse_raw_options(std::span<char const *const> arguments)
{
  raw_options options;
  for (std::size_t index = 0; index < arguments.size(); ++index)
  {
    std::string_view const option{arguments[index]};
    if (option == "--help")
    {
      set_flag_once(options.help, option);
    }
    else if (option == "--list")
    {
      set_flag_once(options.list, option);
    }
    else if (option == "--suite")
    {
      set_once(options.suite, take_value(index, arguments, option), option);
    }
    else if (option == "--benchmark")
    {
      set_once(options.benchmark, take_value(index, arguments, option), option);
    }
    else if (option == "--scene")
    {
      set_once(options.scene, take_value(index, arguments, option), option);
    }
    else if (option == "--rendering")
    {
      set_once(options.rendering, take_value(index, arguments, option), option);
    }
    else if (option == "--width")
    {
      set_once(options.width, take_value(index, arguments, option), option);
    }
    else if (option == "--height")
    {
      set_once(options.height, take_value(index, arguments, option), option);
    }
    else if (option == "--geometry-count")
    {
      set_once(options.geometry_count, take_value(index, arguments, option), option);
    }
    else if (option == "--samples")
    {
      set_once(options.samples, take_value(index, arguments, option), option);
    }
    else if (option == "--precision")
    {
      set_once(options.precision, take_value(index, arguments, option), option);
    }
    else if (option == "--policy")
    {
      set_once(options.policy, take_value(index, arguments, option), option);
    }
    else if (option == "--seed")
    {
      set_once(options.seed, take_value(index, arguments, option), option);
    }
    else if (option == "--warmup")
    {
      set_once(options.warmup, take_value(index, arguments, option), option);
    }
    else if (option == "--repetitions")
    {
      set_once(options.repetitions, take_value(index, arguments, option), option);
    }
    else if (option == "--output")
    {
      set_once(options.output, take_value(index, arguments, option), option);
    }
    else
    {
      throw usage_error("unknown option: " + std::string{option});
    }
  }
  return options;
}


void
require(bool condition, std::string const &message)
{
  if (!condition)
  {
    throw usage_error(message);
  }
}


bool
has_custom_options(raw_options const &options)
{
  return options.scene || options.rendering || options.width || options.height || options.geometry_count ||
         options.samples || options.precision || options.policy || options.seed || options.warmup ||
         options.repetitions;
}

} // namespace htracer::benchmarks::cli
