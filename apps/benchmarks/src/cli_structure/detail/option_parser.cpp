#include <htracer_benchmarks/cli_structure/detail/option_parser.hpp>

#include <htracer_benchmarks/cli_structure/detail/option_descriptor.hpp>
#include <htracer_benchmarks/cli_structure/foundations/errors.hpp>

#include <optional>
#include <string>
#include <string_view>


namespace htracer::benchmarks::cli_structure::detail
{

parsed_token
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
    return {.short_name = std::nullopt, .long_name = name, .attached_value = attached};
  }
  if (argument.size() >= 2 && argument.front() == '-')
  {
    std::optional<std::string_view> attached;
    if (argument.size() > 2)
    {
      attached = argument.substr(argument[2] == '=' ? 3u : 2u);
    }
    return {.short_name = argument[1], .long_name = {}, .attached_value = attached};
  }
  throw usage_error("unexpected argument: " + std::string{argument}, std::string{path});
}

} // namespace htracer::benchmarks::cli_structure::detail
