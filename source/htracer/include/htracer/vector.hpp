#ifndef HTRACER_VECTOR_HPP
#define HTRACER_VECTOR_HPP


#include <htracer/utils/vector_crtp.hpp>

#include <cmath>
#include <numeric>
#include <ostream>


namespace htracer
{
template<typename Float, std::size_t N>
class vector;

template<typename Float, std::size_t N>
std::ostream&
operator<<(std::ostream& os, const vector<Float, N>& rhs);


template<typename Float, std::size_t N>
class vector : private vector_crtp<vector<Float, N>, Float, N>
{
  using VecCrtp = vector_crtp<vector<Float, N>, Float, N>;
  friend VecCrtp;

public:
  using VecCrtp::float_type;
  using VecCrtp::size;

  using iterator = typename VecCrtp::iterator;
  using const_iterator = typename VecCrtp::const_iterator;

  using VecCrtp::vector_crtp;

  using VecCrtp::operator[];

  using VecCrtp::operator+=;
  using VecCrtp::operator-=;
  using VecCrtp::operator*=;

  [[nodiscard]] vector<Float, N>
  normalized() const;

  using VecCrtp::begin;
  using VecCrtp::end;

  using VecCrtp::cbegin;
  using VecCrtp::cend;

  friend std::ostream& operator<<<Float, N>(
      std::ostream& os,
      const vector<Float, N>& rhs);

  void
  swap(vector<Float, N>& rhs) noexcept;
};


template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator+(vector<Float, N> lhs, const vector<Float, N>& rhs);

template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator-(vector<Float, N> lhs, const vector<Float, N>& rhs);

template<typename Float, std::size_t N, typename TConv>
constexpr vector<Float, N>
operator*(vector<Float, N> lhs, TConv scale);

template<typename Float, std::size_t N, typename TConv>
vector<Float, N>
operator*(TConv scale, vector<Float, N> rhs);

template<typename Float, std::size_t N>
constexpr Float
dot(const vector<Float, N>& lhs, const vector<Float, N>& rhs);

template<typename Float>
constexpr vector<Float, 3>
cross(const vector<Float, 3>& lhs, const vector<Float, 3>& rhs);


template<typename Float>
using v3 = vector<Float, 3>;


template<typename Float, std::size_t N>
void
vector<Float, N>::swap(vector<Float, N>& rhs) noexcept
{
  VecCrtp::swap(rhs);
}


template<typename Float, std::size_t N>
vector<Float, N>
vector<Float, N>::normalized() const
{
  const Float norm = std::sqrt(dot(*this, *this));
  return *this * (1 / norm);
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator+(vector<Float, N> lhs, const vector<Float, N>& rhs)
{
  return lhs += rhs;
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator-(vector<Float, N> lhs, const vector<Float, N>& rhs)
{
  return lhs -= rhs;
}


template<typename Float, std::size_t N, typename TConv>
constexpr vector<Float, N>
operator*(vector<Float, N> lhs, TConv scale)
{
  return lhs *= scale;
}


template<typename Float, std::size_t N, typename TConv>
vector<Float, N>
operator*(TConv scale, vector<Float, N> rhs)
{
  return rhs *= scale;
}


template<typename Float, std::size_t N>
constexpr Float
dot(const vector<Float, N>& lhs, const vector<Float, N>& rhs)
{
  return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), Float{0});
}


template<typename Float>
constexpr vector<Float, 3>
cross(const vector<Float, 3>& lhs, const vector<Float, 3>& rhs)
{
  return {
      lhs[1] * rhs[2] - rhs[1] * lhs[2],
      lhs[2] * rhs[0] - rhs[2] * lhs[0],
      lhs[0] * rhs[1] - rhs[0] * lhs[1]};
}


template<typename Float, std::size_t N>
std::ostream&
operator<<(std::ostream& os, const vector<Float, N>& rhs)
{
  os << '[';

  for (std::size_t i = 0; i < N - 1; ++i)
    os << rhs[i] << ',';

  os << rhs[N - 1] << ']';

  return os;
}

} // namespace htracer

#endif // HTRACER_VECTOR_HPP
