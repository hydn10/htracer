#ifndef HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_HELP_WRITER_HPP
#define HTRACER_BENCHMARKS_CLI_STRUCTURE_DETAIL_HELP_WRITER_HPP


#include <htracer_benchmarks/cli_structure/help_page.hpp>

#include <sstream>
#include <string>
#include <string_view>


namespace htracer::benchmarks::cli_structure::detail
{

class help_writer
{
  std::ostringstream output_;

public:
  void
  usage(std::string_view path, std::string_view suffix);

  void
  paragraph(std::string_view text);

  void
  heading(std::string_view text);

  void
  entry(std::string const &label, std::string const &description);

  [[nodiscard]]
  help_page
  finish() &&;
};

} // namespace htracer::benchmarks::cli_structure::detail

#endif
