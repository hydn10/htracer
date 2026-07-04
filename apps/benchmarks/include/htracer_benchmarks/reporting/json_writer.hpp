#ifndef HTRACER_BENCHMARKS_REPORTING_JSON_WRITER_HPP
#define HTRACER_BENCHMARKS_REPORTING_JSON_WRITER_HPP


#include <cstdio>
#include <filesystem>
#include <format>
#include <print>
#include <string_view>
#include <utility>


namespace htracer::benchmarks::reporting
{

class json_writer
{
public:
  explicit json_writer(std::filesystem::path const &path);

  json_writer(json_writer const &) = delete;
  json_writer &
  operator=(json_writer const &) = delete;
  json_writer(json_writer &&) = delete;
  json_writer &
  operator=(json_writer &&) = delete;

  ~json_writer();

  template<typename... Args>
  void
  write(std::format_string<Args...> format, Args &&...args)
  {
    std::print(file_, format, std::forward<Args>(args)...);
  }

  void
  string(std::string_view value);

  void
  close();

private:
  std::FILE *file_{};
};

} // namespace htracer::benchmarks::reporting

#endif
