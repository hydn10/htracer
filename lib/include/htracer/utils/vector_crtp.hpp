#ifndef HTRACER_UTILS_VECTORCRTP_HPP
#define HTRACER_UTILS_VECTORCRTP_HPP


#include <array>
#include <concepts>
#include <ranges>
#include <type_traits>
#include <utility>


namespace htracer::utils
{

namespace detail_
{

template<typename Float, typename... Args>
inline constexpr bool vector_components_nothrow =
    (std::is_nothrow_constructible_v<Float, Args> && ...);

} // namespace detail_


template<typename Derived, typename Float, std::size_t N>
class vector_crtp
{
  std::array<Float, N> elems_;

  friend Derived;

  constexpr vector_crtp() noexcept;
  constexpr explicit vector_crtp(std::array<Float, N> values) noexcept;
  template<typename... Args>
  requires(sizeof...(Args) == N && (std::constructible_from<Float, Args> && ...))
  explicit(sizeof...(Args) == 1) constexpr vector_crtp(Args &&...values) noexcept(
      detail_::vector_components_nothrow<Float, Args...>);

public:
  using float_type = Float;
  static constexpr std::size_t size = N;

  using iterator = decltype(elems_)::iterator;
  using const_iterator = decltype(elems_)::const_iterator;


  template<std::size_t I>
  constexpr Float &
  get() noexcept;

  template<std::size_t I>
  [[nodiscard]]
  constexpr Float const &
  get() const noexcept;

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
  [[nodiscard]]
  constexpr const_iterator
  begin() const noexcept;
  [[nodiscard]]
  constexpr const_iterator
  end() const noexcept;
  [[nodiscard]]
  constexpr const_iterator
  cbegin() const noexcept;
  [[nodiscard]]
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
requires(sizeof...(Args) == N && (std::constructible_from<Float, Args> && ...))
constexpr vector_crtp<Derived, Float, N>::vector_crtp(Args &&...values) noexcept(
    detail_::vector_components_nothrow<Float, Args...>)
    : elems_{static_cast<Float>(std::forward<Args>(values))...}
{
}


template<typename Derived, typename Float, std::size_t N>
constexpr Derived &
vector_crtp<Derived, Float, N>::operator+=(Derived const &rhs) noexcept
{
  for (auto &&[left, right] : std::views::zip(this->elems_, rhs.elems_))
  {
    left += right;
  }

  return static_cast<Derived &>(*this);
}


template<typename Derived, typename Float, std::size_t N>
constexpr Derived &
vector_crtp<Derived, Float, N>::operator-=(Derived const &rhs) noexcept
{
  for (auto &&[left, right] : std::views::zip(this->elems_, rhs.elems_))
  {
    left -= right;
  }

  return static_cast<Derived &>(*this);
}


template<typename Derived, typename Float, std::size_t N>
constexpr Derived &
vector_crtp<Derived, Float, N>::operator*=(Float scale) noexcept
{
  for (auto &element : elems_)
  {
    element *= scale;
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
template<std::size_t I>
constexpr Float &
vector_crtp<Derived, Float, N>::get() noexcept
{
  return std::get<I>(elems_);
}


template<typename Derived, typename Float, std::size_t N>
template<std::size_t I>
constexpr Float const &
vector_crtp<Derived, Float, N>::get() const noexcept
{
  return std::get<I>(elems_);
}

} // namespace htracer::utils

#endif
