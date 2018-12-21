#ifndef HDN_RAYTRACER_VECTOR_HPP
#define HDN_RAYTRACER_VECTOR_HPP


#include <array>
#include <cmath>
#include <ostream>


namespace hdn
{

template <typename T, std::size_t N>
class vector;

template <typename T, std::size_t N>
vector<T, N>
operator+(vector<T, N> lhs, const vector<T, N> &rhs);

template <typename T, std::size_t N>
vector<T, N>
operator-(vector<T, N> lhs, const vector<T, N> &rhs);

template <typename T, std::size_t N>
T
dot(const vector<T, N> &lhs, const vector<T, N> &rhs);

template <typename T, std::size_t N>
std::ostream&
operator<<(std::ostream &os, const vector<T, N> &rhs);


template <typename T, std::size_t N>
class vector
{
  std::array<T, N> elems_;

public:
  vector();
  vector(std::array<T, N> values);
  vector(vector const &v);
  ~vector() = default;

  vector<T, N>&
  operator=(vector<T, N> rhs);

  T&
  operator[](std::size_t index);

  T const&
  operator[](std::size_t index) const;

  vector<T, N>&
  operator+=(const vector<T, N> &rhs);

  vector<T, N>&
  operator-=(const vector<T, N> &rhs);

  vector<T, N>&
  operator*=(T scale);

  vector<T, N>
  normalized();

  friend vector<T, N> operator+<T, N>(vector<T, N> lhs, const vector<T, N> &rhs);
  friend vector<T, N> operator-<T, N>(vector<T, N> lhs, const vector<T, N> &rhs);

  friend std::ostream& operator<<<T, N>(std::ostream &os, const vector<T, N> &rhs);

  void
  swap(vector<T, N> &rhs) noexcept;
};


template <typename T>
using v3 = vector<T, 3>;



template <typename T, std::size_t N>
vector<T, N>::vector()
{

}


template <typename T, std::size_t N>
vector<T, N>::vector(std::array<T, N> values)
  :
    elems_{values}
{

}


template <typename T, std::size_t N>
vector<T, N>::vector(vector const &v)
  :
    elems_{v.elems_}
{

}


template <typename T, std::size_t N>
vector<T, N>&
vector<T, N>::operator=(vector<T, N> rhs)
{
  this->swap(rhs);
  return *this;
}


template <typename T, std::size_t N>
vector<T, N>&
vector<T, N>::operator+=(const vector<T, N> &rhs)
{
  for (std::size_t i = 0; i < N; ++i)
    this->elems_[i] += rhs.elems_[i];

  return *this;
}


template <typename T, std::size_t N>
vector<T, N>&
vector<T, N>::operator-=(const vector<T, N> &rhs)
{
  for (std::size_t i = 0; i < N; ++i)
    this->elems_[i] -= rhs.elems_[i];

  return *this;
}


template <typename T, std::size_t N>
vector<T, N>&
vector<T, N>::operator*=(T scale)
{
  for (std::size_t i = 0; i < N; ++i)
    elems_[i] *= scale;

  return *this;
}


template <typename T, std::size_t N>
void
vector<T, N>::swap(vector<T, N> &rhs) noexcept
{
  std::swap(this->elems_, rhs.elems_);
}


template <typename T, std::size_t N>
T&
vector<T, N>::operator[](std::size_t index)
{
  return elems_[index];
}


template <typename T, std::size_t N>
T const&
vector<T, N>::operator[](std::size_t index) const
{
  return elems_[index];
}


template <typename T, std::size_t N>
vector<T, N>
vector<T, N>::normalized()
{
  vector<T, N> result;

  auto norm = std::sqrt(dot(*this, *this));

  for (std::size_t i = 0; i < N; ++i)
    result[i] = elems_[i] / norm;

  return result;
}



template <typename T, std::size_t N>
vector<T, N>
operator+(vector<T, N> lhs, const vector<T, N> &rhs)
{
  lhs += rhs;
  return lhs;
}


template <typename T, std::size_t N>
vector<T, N>
operator-(vector<T, N> lhs, const vector<T, N> &rhs)
{
  lhs -= rhs;
  return lhs;
}


template <typename T, std::size_t N, typename TConv>
vector<T, N>
operator*(vector<T, N> lhs, TConv scale)
{
  lhs *= scale;
  return lhs;
}


template <typename T, std::size_t N>
T
dot(const vector<T, N> &lhs, const vector<T, N> &rhs)
{
  T result{0};

  for (std::size_t i = 0; i < N; ++i)
    result += lhs[i] * rhs[i];

  return result;
}


template <typename T>
vector<T, 3>
cross(const vector<T, 3> &lhs, const vector<T, 3> &rhs)
{
  return vector<T, 3>({
      lhs[1]*rhs[2] - rhs[1]*lhs[2],
      lhs[2]*rhs[0] - rhs[2]*lhs[0],
      lhs[0]*rhs[1] - rhs[0]*lhs[1]});
}


template <typename T, std::size_t N>
std::ostream&
operator<<(std::ostream &os, const vector<T, N> &rhs)
{
  os << '[';

  for (std::size_t i = 0; i < N - 1; ++i)
    os << rhs.elems_[i] << ',';

  os << rhs.elems_[N-1] << ']';

  return os;
}

} // namespace hdn

#endif // HDN_RAYTRACER_VECTOR_HPP
