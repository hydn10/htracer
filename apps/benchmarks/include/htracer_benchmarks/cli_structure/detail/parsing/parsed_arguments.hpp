#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_PARSING_PARSED_ARGUMENTS_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_PARSING_PARSED_ARGUMENTS_HPP


#include <htracer_benchmarks/cli_structure/detail/parsing/argument_view.hpp>
#include <htracer_benchmarks/cli_structure/detail/parsing/option_descriptor.hpp>
#include <htracer_benchmarks/cli_structure/errors.hpp>

#include <algorithm>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>


namespace htracer::benchmarks::cli_structure::detail::parsing
{

class parsed_arguments
{
  std::vector<std::string_view> flags_;
  std::vector<std::pair<std::string_view, std::string_view>> values_;

  explicit parsed_arguments(
      std::vector<std::string_view> flags, std::vector<std::pair<std::string_view, std::string_view>> values);

public:
  class producer;

  [[nodiscard]]
  bool
  contains(std::string_view name) const noexcept;

  [[nodiscard]]
  std::optional<std::string_view>
  value(std::string_view name) const noexcept;
};


class parsed_arguments::producer
{
  std::vector<std::string_view> flags_;
  std::vector<std::pair<std::string_view, std::string_view>> values_;

  [[nodiscard]]
  bool
  contains(std::string_view name) const noexcept;

public:
  explicit producer(std::size_t capacity);

  void
  record(
      option_descriptor const &descriptor,
      std::optional<std::string_view> attached_value,
      argument_view &remaining,
      std::string_view path);

  [[nodiscard]]
  parsed_arguments
  finish() &&;
};


inline parsed_arguments::producer::producer(std::size_t capacity)
{
  flags_.reserve(capacity);
  values_.reserve(capacity);
}


inline bool
parsed_arguments::producer::contains(std::string_view name) const noexcept
{
  return std::ranges::any_of(
             flags_,
             [name](std::string_view flag)
             {
               return flag == name;
             }) ||
         std::ranges::any_of(
             values_,
             [name](auto const &value)
             {
               return value.first == name;
             });
}


inline void
parsed_arguments::producer::record(
    option_descriptor const &descriptor,
    std::optional<std::string_view> attached_value,
    argument_view &remaining,
    std::string_view path)
{
  auto const long_name = descriptor.name().long_name();
  if (contains(long_name))
  {
    throw usage_error("duplicate option: --" + std::string{long_name}, std::string{path});
  }

  if (!descriptor.takes_value())
  {
    if (attached_value)
    {
      throw usage_error("option does not accept a value: --" + std::string{long_name}, std::string{path});
    }
    flags_.push_back(long_name);
    return;
  }

  std::string_view value;
  if (attached_value)
  {
    value = *attached_value;
  }
  else
  {
    if (remaining.empty())
    {
      throw usage_error("missing value for option: --" + std::string{long_name}, std::string{path});
    }
    value = remaining[0];
    remaining = remaining.subspan(1);
    if (value.starts_with('-'))
    {
      throw usage_error("missing value for option: --" + std::string{long_name}, std::string{path});
    }
  }

  if (value.empty())
  {
    throw usage_error("missing value for option: --" + std::string{long_name}, std::string{path});
  }
  values_.emplace_back(long_name, value);
}


inline parsed_arguments
parsed_arguments::producer::finish() &&
{
  return parsed_arguments{std::move(flags_), std::move(values_)};
}

} // namespace htracer::benchmarks::cli_structure::detail::parsing

#endif
