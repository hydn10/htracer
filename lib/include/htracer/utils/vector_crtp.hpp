#ifndef HTRACER_UTILS_VECTORCRTP_HPP
#define HTRACER_UTILS_VECTORCRTP_HPP


#include <array>


namespace htracer::utils
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

  constexpr vector_crtp() noexcept;
  constexpr explicit vector_crtp(std::array<Float, N> values) noexcept;
  template<typename... Args>
  constexpr vector_crtp(Args... values) noexcept;
  virtual ~vector_crtp() = default;

  constexpr Float &
  operator[](std::size_t index) noexcept;

  [[nodiscard]]
  constexpr Float const &
  operator[](std::size_t index) const noexcept;

  constexpr Derived &
  operator+=(Derived const &rhs) noexcept;
  constexpr Derived &
  operator-=(Derived const &rhs) noexcept;
  constexpr Derived &
  operator*=(Float scale) noexcept;

  constexpr iterator
  begin() noexcept;
  constexpr iterator
  end() noexcept;
  constexpr const_iterator
  begin() const noexcept;
  constexpr const_iterator
  end() const noexcept;
  constexpr const_iterator
  cbegin() const noexcept;
  constexpr const_iterator
  cend() const noexcept;

  constexpr void
  swap(Derived &rhs) noexcept;
};


template<typename Derived, typename Float, std::size_t N>
constexpr vector_crtp<Derived, Float, N>::vector_crtp() noexcept
    : elems_{}
{
}


template<typename Derived, typename Float, std::size_t N>
constexpr vector_crtp<Derived, Float, N>::vector_crtp(std::array<Float, N> values) noexcept
    : elems_{values}
{
}


template<typename Derived, typename Float, std::size_t N>
template<typename... Args>
constexpr vector_crtp<Derived, Float, N>::vector_crtp(Args... values) noexcept
    : elems_{static_cast<Float>(values)...}
{
}


template<typename Derived, typename Float, std::size_t N>
constexpr Derived &
vector_crtp<Derived, Float, N>::operator+=(Derived const &rhs) noexcept
{
  for (std::size_t i = 0; i < N; ++i)
  {
    this->elems_[i] += rhs.elems_[i];
  }

  return static_cast<Derived &>(*this);
}


template<typename Derived, typename Float, std::size_t N>
constexpr Derived &
vector_crtp<Derived, Float, N>::operator-=(Derived const &rhs) noexcept
{
  for (std::size_t i = 0; i < N; ++i)
  {
    this->elems_[i] -= rhs.elems_[i];
  }

  return static_cast<Derived &>(*this);
}


template<typename Derived, typename Float, std::size_t N>
constexpr Derived &
vector_crtp<Derived, Float, N>::operator*=(Float scale) noexcept
{
  for (std::size_t i = 0; i < N; ++i)
  {
    elems_[i] *= scale;
  }

  return static_cast<Derived &>(*this);
}


template<typename Derived, typename Float, std::size_t N>
constexpr auto
vector_crtp<Derived, Float, N>::begin() noexcept -> iterator
{
  return elems_.begin();
}


template<typename Derived, typename Float, std::size_t N>
constexpr auto
vector_crtp<Derived, Float, N>::end() noexcept -> iterator
{
  return elems_.end();
}


template<typename Derived, typename Float, std::size_t N>
constexpr auto
vector_crtp<Derived, Float, N>::begin() const noexcept -> const_iterator
{
  return elems_.begin();
}


template<typename Derived, typename Float, std::size_t N>
constexpr auto
vector_crtp<Derived, Float, N>::end() const noexcept -> const_iterator
{
  return elems_.end();
}


template<typename Derived, typename Float, std::size_t N>
constexpr auto
vector_crtp<Derived, Float, N>::cbegin() const noexcept -> const_iterator
{
  return elems_.cbegin();
}


template<typename Derived, typename Float, std::size_t N>
constexpr auto
vector_crtp<Derived, Float, N>::cend() const noexcept -> const_iterator
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
vector_crtp<Derived, Float, N>::operator[](std::size_t index) noexcept
{
  return elems_[index];
}


template<typename Derived, typename Float, std::size_t N>
constexpr Float const &
vector_crtp<Derived, Float, N>::operator[](std::size_t index) const noexcept
{
  return elems_[index];
}

} // namespace htracer::utils

#endif