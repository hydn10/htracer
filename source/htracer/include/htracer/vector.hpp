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
std::ostream &
operator<<(std::ostream &os, vector<Float, N> const &rhs);


template<typename Float, std::size_t N>
class vector final : private utils::vector_crtp<vector<Float, N>, Float, N>
{
  using VecCrtp = utils::vector_crtp<vector<Float, N>, Float, N>;
  friend VecCrtp;

public:
  using typename VecCrtp::float_type;
  using VecCrtp::size;

  using iterator = typename VecCrtp::iterator;
  using const_iterator = typename VecCrtp::const_iterator;

  // TODO: For some reason clang++ doesnt like VecCrtp::vector_crtp
  using utils::vector_crtp<vector<Float, N>, Float, N>::vector_crtp;

  using VecCrtp::operator[];

  using VecCrtp::operator+=;
  using VecCrtp::operator-=;
  using VecCrtp::operator*=;

  using VecCrtp::begin;
  using VecCrtp::end;

  using VecCrtp::cbegin;
  using VecCrtp::cend;

  void
  swap(vector<Float, N> &rhs) noexcept;
};


template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator+(vector<Float, N> lhs, vector<Float, N> const &rhs);

template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator-(vector<Float, N> lhs, vector<Float, N> const &rhs);

template<typename Float, std::size_t N, typename TConv>
constexpr vector<Float, N>
operator*(vector<Float, N> lhs, TConv scale);

template<typename Float, std::size_t N, typename TConv>
vector<Float, N>
operator*(TConv scale, vector<Float, N> rhs);

template<typename Float, std::size_t N>
constexpr Float
dot(vector<Float, N> const &lhs, vector<Float, N> const &rhs);

template<typename Float, std::size_t N>
constexpr vector<Float, N>
normalize(vector<Float, N> const &v);

template<typename Float>
constexpr vector<Float, 3>
cross(vector<Float, 3> const &lhs, vector<Float, 3> const &rhs);


template<typename Float>
using v3 = vector<Float, 3>;


template<typename Float, std::size_t N>
void
vector<Float, N>::swap(vector<Float, N> &rhs) noexcept
{
  VecCrtp::swap(rhs);
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator+(vector<Float, N> lhs, vector<Float, N> const &rhs)
{
  return lhs += rhs;
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator-(vector<Float, N> lhs, vector<Float, N> const &rhs)
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
dot(vector<Float, N> const &lhs, vector<Float, N> const &rhs)
{
  return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), Float{0});
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
normalize(vector<Float, N> const &v)
{
  auto const norm = std::sqrt(dot(v, v));
  return v * (1 / norm);
}


template<typename Float>
constexpr vector<Float, 3>
cross(vector<Float, 3> const &lhs, vector<Float, 3> const &rhs)
{
  return {lhs[1] * rhs[2] - rhs[1] * lhs[2], lhs[2] * rhs[0] - rhs[2] * lhs[0], lhs[0] * rhs[1] - rhs[0] * lhs[1]};
}

} // namespace htracer

#endif