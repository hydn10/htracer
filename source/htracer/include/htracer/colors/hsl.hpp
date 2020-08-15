#ifndef HTRACER_COLORS_HSL_HPP
#define HTRACER_COLORS_HSL_HPP


#include <htracer/vector.hpp>


namespace htracer::colors
{
template<typename Float>
class hsl : private v3<Float>
{
};

} // namespace htracer::colors

#endif // HTRACER_COLORS_HSL_HPP
