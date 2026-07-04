#include <htracer_benchmarks/reporting/json_writer.hpp>

#include <cerrno>
#include <cstdio>
#include <filesystem>
#include <string_view>
#include <system_error>
#include <utility>


namespace htracer::benchmarks::reporting
{

json_writer::json_writer(std::filesystem::path const &path)
{
#if defined(_WIN32)
  if (_wfopen_s(&file_, path.c_str(), L"wb") != 0) // NOLINT(misc-include-cleaner)
  {
    file_ = nullptr;
  }
#else
  file_ = std::fopen(path.c_str(), "wb");
#endif
  if (file_ == nullptr)
  {
    throw std::system_error(errno, std::generic_category(), "failed to open JSON output");
  }
}


json_writer::~json_writer()
{
  if (file_ != nullptr)
  {
    (void)std::fclose(file_); // NOLINT(cppcoreguidelines-owning-memory)
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
        write("\\u{:04x}", static_cast<unsigned>(character));
      }
      else if (std::fputc(character, file_) == EOF)
      {
        throw std::system_error(errno, std::generic_category(), "failed to write JSON output");
      }
    }
  }
  write("\"");
}


void
json_writer::close()
{
  auto *const file = std::exchange(file_, nullptr);
  if (std::fclose(file) != 0) // NOLINT(cppcoreguidelines-owning-memory)
  {
    throw std::system_error(errno, std::generic_category(), "failed to close JSON output");
  }
}

} // namespace htracer::benchmarks::reporting
