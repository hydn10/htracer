#ifndef HTRACER_BENCHMARKS_REPORTING_JSON_WRITER_HPP
#define HTRACER_BENCHMARKS_REPORTING_JSON_WRITER_HPP


#include <filesystem>
#include <fstream>
#include <string_view>


namespace htracer::benchmarks::reporting
{

class json_writer
{
  std::ofstream file_;

public:
  explicit json_writer(std::filesystem::path const &path);

  json_writer(json_writer const &) = delete;
  json_writer &
  operator=(json_writer const &) = delete;
  json_writer(json_writer &&) = delete;
  json_writer &
  operator=(json_writer &&) = delete;

  void
  write(std::string_view value);

  void
  string(std::string_view value);

  void
  close();
};

} // namespace htracer::benchmarks::reporting

#endif
