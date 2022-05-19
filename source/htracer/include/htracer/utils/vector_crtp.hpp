#ifndef HTRACER_UTILS_VECTORCRTP_HPP
#define HTRACER_UTILS_VECTORCRTP_HPP


#include <array>


namespace htracer
{

template<typename Derived, typename Float, std::size_t N>
class vector_crtp
{
  std::array<Float, N> elems_;

public:
  using float_type = Float;
  static constexpr std::size_t size = N;

  using iterator = typename decltype(elems_)::iterator;
  using const_iterator = typename decltype(elems_)::const_iterator;

  constexpr vector_crtp() = default;
  constexpr explicit vector_crtp(std::array<Float, N> values);
  template<typename... Args>
  constexpr vector_crtp(Args... values);
  virtual ~vector_crtp() = default;

  constexpr Float &
  operator[](std::size_t index);

  constexpr Float const &
  operator[](std::size_t index) const;

  constexpr Derived &
  operator+=(Derived const &rhs);
  constexpr Derived &
  operator-=(Derived const &rhs);
  constexpr Derived &
  operator*=(Float scale);

  constexpr iterator
  begin();
  constexpr iterator
  end();
  constexpr const_iterator
  begin() const;
  constexpr const_iterator
  end() const;
  constexpr const_iterator
  cbegin() const;
  constexpr const_iterator
  cend() const;

  constexpr void
  swap(Derived &rhs) noexcept;
};


template<typename Derived, typename Float, std::size_t N>
constexpr vector_crtp<Derived, Float, N>::vector_crtp(std::array<Float, N> values)
    : elems_{values}
{
}


template<typename Derived, typename Float, std::size_t N>
template<typename... Args>
constexpr vector_crtp<Derived, Float, N>::vector_crtp(Args... values)
    : elems_{static_cast<Float>(values)...}
{
}


template<typename Derived, typename Float, std::size_t N>
constexpr Derived &
vector_crtp<Derived, Float, N>::operator+=(Derived const &rhs)
{
  for (std::size_t i = 0; i < N; ++i)
    this->elems_[i] += rhs.elems_[i];

  return static_cast<Derived &>(*this);
}


template<typename Derived, typename Float, std::size_t N>
constexpr Derived &
vector_crtp<Derived, Float, N>::operator-=(Derived const &rhs)
{
  for (std::size_t i = 0; i < N; ++i)
    this->elems_[i] -= rhs.elems_[i];

  return static_cast<Derived &>(*this);
}


template<typename Derived, typename Float, std::size_t N>
constexpr Derived &
vector_crtp<Derived, Float, N>::operator*=(Float scale)
{
  for (std::size_t i = 0; i < N; ++i)
    elems_[i] *= scale;

  return static_cast<Derived &>(*this);
}


template<typename Derived, typename Float, std::size_t N>
constexpr auto
vector_crtp<Derived, Float, N>::begin() -> iterator
{
  return elems_.begin();
}


template<typename Derived, typename Float, std::size_t N>
constexpr auto
vector_crtp<Derived, Float, N>::end() -> iterator
{
  return elems_.end();
}


template<typename Derived, typename Float, std::size_t N>
constexpr auto
vector_crtp<Derived, Float, N>::begin() const -> const_iterator
{
  return elems_.begin();
}


template<typename Derived, typename Float, std::size_t N>
constexpr auto
vector_crtp<Derived, Float, N>::end() const -> const_iterator
{
  return elems_.end();
}


template<typename Derived, typename Float, std::size_t N>
constexpr auto
vector_crtp<Derived, Float, N>::cbegin() const -> const_iterator
{
  return elems_.cbegin();
}


template<typename Derived, typename Float, std::size_t N>
constexpr auto
vector_crtp<Derived, Float, N>::cend() const -> const_iterator
{
  return elems_.cend();
}


template<typename Derived, typename Float, std::size_t N>
constexpr void
vector_crtp<Derived, Float, N>::swap(Derived &rhs) noexcept
{
  std::swap(this->elems_, rhs.elems_);
}


template<typename Derived, typename Float, std::size_t N>
constexpr Float &
vector_crtp<Derived, Float, N>::operator[](std::size_t index)
{
  return elems_[index];
}


template<typename Derived, typename Float, std::size_t N>
constexpr Float const &
vector_crtp<Derived, Float, N>::operator[](std::size_t index) const
{
  return elems_[index];
}

} // namespace htracer

#endif