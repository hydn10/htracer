#include <htracer_benchmarks/cli_structure/detail/parsing/option_parser.hpp>

#include <htracer_benchmarks/cli_structure/detail/parsing/option_descriptor.hpp>
#include <htracer_benchmarks/cli_structure/errors.hpp>

#include <concepts>
#include <optional>
#include <string>
#include <string_view>
#include <variant>


namespace htracer::benchmarks::cli_structure::detail::parsing
{

option_token::option_token(char name, std::optional<std::string_view> attached_value)
    : name_{name}
    , attached_value_{attached_value}
{
}


option_token::option_token(std::string_view name, std::optional<std::string_view> attached_value)
    : name_{name}
    , attached_value_{attached_value}
{
}


bool
option_token::matches(option_descriptor const &descriptor) const
{
  return std::visit(
      [descriptor](auto name)
      {
        if constexpr (std::same_as<decltype(name), char>)
        {
          return name == descriptor.name().short_name();
        }
        else
        {
          return name == descriptor.name().long_name();
        }
      },
      name_);
}


std::optional<std::string_view>
option_token::attached_value() const noexcept
{
  return attached_value_;
}


option_token
parse_token(std::string_view argument, std::string_view path)
{
  if (argument.starts_with("--"))
  {
    auto const separator = argument.find('=');
    auto const name = argument.substr(2, separator == std::string_view::npos ? separator : separator - 2);
    if (name.empty())
    {
      throw usage_error("invalid option: " + std::string{argument}, std::string{path});
    }
    auto const attached = separator == std::string_view::npos ? std::optional<std::string_view>{}
                                                              : std::optional{argument.substr(separator + 1)};
    return option_token{name, attached};
  }
  if (argument.size() >= 2 && argument.front() == '-')
  {
    std::optional<std::string_view> attached;
    if (argument.size() > 2)
    {
      attached = argument.substr(argument[2] == '=' ? 3u : 2u);
    }
    return option_token{argument[1], attached};
  }
  throw usage_error("unexpected argument: " + std::string{argument}, std::string{path});
}

} // namespace htracer::benchmarks::cli_structure::detail::parsing
