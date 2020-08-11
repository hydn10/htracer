#ifndef HTRACER_VECTOR_HPP
#define HTRACER_VECTOR_HPP


#include <array>
#include <cmath>
#include <numeric>
#include <ostream>


namespace htracer
{
template<typename Float, std::size_t N>
class vector;

template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator+(vector<Float, N> lhs, const vector<Float, N>& rhs);

template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator-(vector<Float, N> lhs, const vector<Float, N>& rhs);

template<typename Float, std::size_t N>
constexpr Float
dot(const vector<Float, N>& lhs, const vector<Float, N>& rhs);

template<typename Float, std::size_t N>
std::ostream&
operator<<(std::ostream& os, const vector<Float, N>& rhs);


template<typename Float, std::size_t N>
class vector
{
  std::array<Float, N> elems_;

public:
  using iterator = typename decltype(elems_)::iterator;
  using const_iterator = typename decltype(elems_)::const_iterator;

  constexpr vector() = default;
  constexpr explicit vector(std::array<Float, N> values);
  template<typename... Args>
  constexpr vector(Args... values);

  Float&
  operator[](std::size_t index);

  Float const&
  operator[](std::size_t index) const;

  constexpr vector<Float, N>&
  operator+=(const vector<Float, N>& rhs);

  constexpr vector<Float, N>&
  operator-=(const vector<Float, N>& rhs);

  constexpr vector<Float, N>&
  operator*=(Float scale);

  vector<Float, N>
  normalized() const;

  auto
  begin();
  auto
  end();
  auto
  cbegin() const;
  auto
  cend() const;

  friend Float
  dot<Float, N>(const vector<Float, N>& lhs, const vector<Float, N>& rhs);

  friend vector<Float, N> operator+<Float, N>(
      vector<Float, N> lhs,
      const vector<Float, N>& rhs);

  friend vector<Float, N> operator-<Float, N>(
      vector<Float, N> lhs,
      const vector<Float, N>& rhs);

  friend std::ostream& operator<<<Float, N>(
      std::ostream& os,
      const vector<Float, N>& rhs);

  void
  swap(vector<Float, N>& rhs) noexcept;
};


template<typename Float>
using v3 = vector<Float, 3>;


template<typename Float, std::size_t N>
constexpr vector<Float, N>::vector(std::array<Float, N> values) : elems_{values}
{
}


template<typename Float, std::size_t N>
template<typename... Args>
constexpr vector<Float, N>::vector(Args... values) : elems_{values...}
{
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>&
vector<Float, N>::operator+=(const vector<Float, N>& rhs)
{
  for (std::size_t i = 0; i < N; ++i)
    this->elems_[i] += rhs.elems_[i];

  return *this;
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>&
vector<Float, N>::operator-=(const vector<Float, N>& rhs)
{
  for (std::size_t i = 0; i < N; ++i)
    this->elems_[i] -= rhs.elems_[i];

  return *this;
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>&
vector<Float, N>::operator*=(Float scale)
{
  for (std::size_t i = 0; i < N; ++i)
    elems_[i] *= scale;

  return *this;
}


template<typename Float, std::size_t N>
auto
vector<Float, N>::begin()
{
  return elems_.begin();
}


template<typename Float, std::size_t N>
auto
vector<Float, N>::end()
{
  return elems_.end();
}


template<typename Float, std::size_t N>
auto
vector<Float, N>::cbegin() const
{
  return elems_.cbegin();
}


template<typename Float, std::size_t N>
auto
vector<Float, N>::cend() const
{
  return elems_.cend();
}


template<typename Float, std::size_t N>
void
vector<Float, N>::swap(vector<Float, N>& rhs) noexcept
{
  std::swap(this->elems_, rhs.elems_);
}


template<typename Float, std::size_t N>
Float&
vector<Float, N>::operator[](std::size_t index)
{
  return elems_[index];
}


template<typename Float, std::size_t N>
Float const&
vector<Float, N>::operator[](std::size_t index) const
{
  return elems_[index];
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
  lhs += rhs;
  return lhs;
}


template<typename Float, std::size_t N>
constexpr vector<Float, N>
operator-(vector<Float, N> lhs, const vector<Float, N>& rhs)
{
  lhs -= rhs;
  return lhs;
}


template<typename Float, std::size_t N, typename TConv>
constexpr vector<Float, N>
operator*(vector<Float, N> lhs, TConv scale)
{
  lhs *= scale;
  return lhs;
}


template<typename Float, std::size_t N, typename TConv>
vector<Float, N>
operator*(TConv scale, vector<Float, N> rhs)
{
  rhs *= scale;
  return rhs;
}


template<typename Float, std::size_t N>
constexpr Float
dot(const vector<Float, N>& lhs, const vector<Float, N>& rhs)
{
  return std::inner_product(
      lhs.elems_.cbegin(), lhs.elems_.cend(), rhs.elems_.cbegin(), Float{0});
}


template<typename Float>
vector<Float, 3>
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
    os << rhs.elems_[i] << ',';

  os << rhs.elems_[N - 1] << ']';

  return os;
}

} // namespace htracer

#endif // HTRACER_VECTOR_HPP
