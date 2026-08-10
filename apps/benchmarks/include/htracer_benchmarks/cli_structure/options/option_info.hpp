#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_OPTIONS_OPTION_INFO_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_OPTIONS_OPTION_INFO_HPP


#include <htracer_benchmarks/cli_structure/detail/name_validation.hpp>
#include <htracer_benchmarks/cli_structure/foundations/errors.hpp>

#include <string_view>


namespace htracer::benchmarks::cli_structure
{

class option_name
{
  char short_;
  std::string_view long_;

public:
  constexpr option_name(char short_name, std::string_view long_name);

  [[nodiscard]]
  constexpr char
  short_name() const noexcept;

  [[nodiscard]]
  constexpr std::string_view
  long_name() const noexcept;
};

class flag_info
{
  option_name name_;
  std::string_view description_;

public:
  constexpr flag_info(option_name name, std::string_view description);

  [[nodiscard]]
  constexpr option_name
  name() const noexcept;

  [[nodiscard]]
  constexpr std::string_view
  description() const noexcept;
};


class value_option_info
{
  option_name name_;
  std::string_view value_name_;
  std::string_view description_;

public:
  constexpr value_option_info(option_name name, std::string_view value_name, std::string_view description);

  [[nodiscard]]
  constexpr option_name
  name() const noexcept;

  [[nodiscard]]
  constexpr std::string_view
  value_name() const noexcept;

  [[nodiscard]]
  constexpr std::string_view
  description() const noexcept;
};

constexpr option_name::option_name(char short_name, std::string_view long_name)
    : short_{short_name}
    , long_{long_name}
{
  auto const alphanumeric = (short_name >= 'a' && short_name <= 'z') || (short_name >= 'A' && short_name <= 'Z') ||
                            (short_name >= '0' && short_name <= '9');
  if (!alphanumeric || short_name == 'h' || long_name == "help" || !detail::valid_name(long_name))
  {
    throw schema_error{"invalid or reserved option name"};
  }
}


constexpr char
option_name::short_name() const noexcept
{
  return short_;
}


constexpr std::string_view
option_name::long_name() const noexcept
{
  return long_;
}


constexpr flag_info::flag_info(option_name name, std::string_view description)
    : name_{name}
    , description_{description}
{
  if (description.empty())
  {
    throw schema_error{"an option description cannot be empty"};
  }
}


constexpr option_name
flag_info::name() const noexcept
{
  return name_;
}


constexpr std::string_view
flag_info::description() const noexcept
{
  return description_;
}


constexpr value_option_info::value_option_info(
    option_name name, std::string_view value_name, std::string_view description)
    : name_{name}
    , value_name_{value_name}
    , description_{description}
{
  if (value_name.empty() || description.empty())
  {
    throw schema_error{"an option value name and description cannot be empty"};
  }
}


constexpr option_name
value_option_info::name() const noexcept
{
  return name_;
}


constexpr std::string_view
value_option_info::value_name() const noexcept
{
  return value_name_;
}


constexpr std::string_view
value_option_info::description() const noexcept
{
  return description_;
}

} // namespace htracer::benchmarks::cli_structure

#endif
