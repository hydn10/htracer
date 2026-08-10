#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_FOUNDATIONS_ERRORS_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_FOUNDATIONS_ERRORS_HPP


#include <stdexcept>
#include <string>


namespace htracer::benchmarks::cli_structure
{

class usage_error : public std::runtime_error
{
  std::string help_path_;

public:
  usage_error(std::string const &message, std::string help_path);

  [[nodiscard]]
  std::string const &
  help_path() const noexcept;
};


class schema_error : public std::logic_error
{
public:
  using std::logic_error::logic_error;
};

} // namespace htracer::benchmarks::cli_structure

#endif
