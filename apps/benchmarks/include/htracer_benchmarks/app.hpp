#ifndef HTRACER_BENCHMARKS_APP_HPP
#define HTRACER_BENCHMARKS_APP_HPP


#include <span>


namespace htracer::benchmarks
{

void
run_app(std::span<char const *const> arguments);

} // namespace htracer::benchmarks

#endif
