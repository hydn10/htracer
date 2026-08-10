#include <htracer_benchmarks/cli_structure/detail/help_writer.hpp>

#include <htracer_benchmarks/cli_structure/foundations/help_page.hpp>

#include <iomanip>
#include <ios>
#include <string>
#include <string_view>
#include <utility>


namespace htracer::benchmarks::cli_structure::detail
{

void
help_writer::usage(std::string_view path, std::string_view suffix)
{
  output_ << "Usage:\n  " << path << suffix << "\n";
}


void
help_writer::paragraph(std::string_view text)
{
  output_ << '\n' << text << "\n";
}


void
help_writer::heading(std::string_view text)
{
  output_ << '\n' << text << ":\n";
}


void
help_writer::entry(std::string const &label, std::string const &description)
{
  output_ << "  " << std::left << std::setw(38) << label << description << '\n';
}


help_page
help_writer::finish() &&
{
  return {.text = std::move(output_).str()};
}

} // namespace htracer::benchmarks::cli_structure::detail
