#include <htracer_benchmarks/reporting/json_writer.hpp>

#include <filesystem>
#include <format>
#include <ios>
#include <string_view>


namespace htracer::benchmarks::reporting
{

json_writer::json_writer(std::filesystem::path const &path)
    : file_(path, std::ios::binary)
{
  if (!file_)
  {
    throw std::ios_base::failure("failed to open JSON output");
  }
}


void
json_writer::write(std::string_view value)
{
  file_.write(value.data(), static_cast<std::streamsize>(value.size()));
  if (!file_)
  {
    throw std::ios_base::failure("failed to write JSON output");
  }
}


void
json_writer::string(std::string_view value)
{
  write("\"");
  for (unsigned char const character : value)
  {
    switch (character)
    {
    case '"':
      write("\\\"");
      break;
    case '\\':
      write("\\\\");
      break;
    case '\b':
      write("\\b");
      break;
    case '\f':
      write("\\f");
      break;
    case '\n':
      write("\\n");
      break;
    case '\r':
      write("\\r");
      break;
    case '\t':
      write("\\t");
      break;
    default:
      if (character < 0x20)
      {
        write(std::format("\\u{:04x}", static_cast<unsigned>(character)));
      }
      else
      {
        file_.put(static_cast<char>(character));
        if (!file_)
        {
          throw std::ios_base::failure("failed to write JSON output");
        }
      }
    }
  }
  write("\"");
}


void
json_writer::close()
{
  if (file_.is_open())
  {
    file_.close();
    if (!file_)
    {
      throw std::ios_base::failure("failed to close JSON output");
    }
  }
}

} // namespace htracer::benchmarks::reporting
