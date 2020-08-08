#ifndef HTRACER_UTILS_CONTAINER_HPP
#define HTRACER_UTILS_CONTAINER_HPP


#include <functional>
#include <variant>
#include <vector>


namespace htracer::utils
{
namespace detail_
{
template<unsigned N, typename First, typename... Remaining>
struct type_at
{
  using type = typename type_at<N - 1, Remaining...>::type;
};


template<typename First, typename... Remaining>
struct type_at<0, First, Remaining...>
{
  using type = First;
};


template<typename Container>
class single_container
{
public:
  Container cont;
};
} // namespace detail_


template<typename... T>
class container : private detail_::single_container<std::vector<T>>...
{
  template<typename U>
  using u_container = typename std::vector<U>;

  template<typename U>
  using u_iterator = typename u_container<U>::iterator;

  template<typename U>
  using u_const_iterator = typename u_container<U>::const_iterator;

  template<std::size_t N>
  using nth_type = typename detail_::type_at<N, T...>::type;

  template<std::size_t N>
  using nth_container = u_container<nth_type<N>>;

  template<std::size_t N>
  using nth_container_iterator = typename nth_container<N>::iterator;

  template<std::size_t N>
  using nth_container_const_iterator =
      typename nth_container<N>::const_iterator;

  template<typename U>
  constexpr auto&
  as() noexcept;

  template<typename U>
  [[nodiscard]] constexpr const auto&
  as() const noexcept;

  template<size_t N>
  constexpr auto&
  as() noexcept;

  template<size_t N>
  [[nodiscard]] constexpr const auto&
  as() const noexcept;

  template<typename U>
  auto&
  get_cont() noexcept;

  template<typename U>
  [[nodiscard]] const auto&
  get_cont() const noexcept;

public:
  using reference = std::variant<std::reference_wrapper<T>...>;
  using const_reference = std::variant<std::reference_wrapper<const T>...>;

  template<typename... Elems>
  void
  push(const Elems&... elems);

  template<typename... Elems>
  void
  push(Elems&&... elems);

  template<typename U, typename... Args>
  typename u_container<U>::reference
  emplace(Args&&... args);

  class const_iterator
  {
    friend class container<T...>;

    const container<T...>* cont_;
    std::variant<u_const_iterator<T>...> curr_iter_;

    explicit const_iterator(const container<T...>& container) noexcept;
    const_iterator(
        const container<T...>& container,
        std::variant<u_const_iterator<T>...> iterator) noexcept;

  public:
    const_reference
    operator*() const noexcept;
    bool
    operator==(const const_iterator& rhs) const noexcept;
    bool
    operator!=(const const_iterator& rhs) const noexcept;
    const_iterator&
    operator++() noexcept;
  };

  const_iterator
  cbegin() const noexcept;
  const_iterator
  cend() const noexcept;
};


template<typename... T>
template<typename U>
constexpr auto&
container<T...>::as() noexcept
{
  return static_cast<detail_::single_container<u_container<U>>&>(*this);
}


template<typename... T>
template<typename U>
[[nodiscard]] constexpr const auto&
container<T...>::as() const noexcept
{
  return static_cast<const detail_::single_container<u_container<U>>&>(*this);
}


template<typename... T>
template<size_t N>
constexpr auto&
container<T...>::as() noexcept
{
  return as<nth_type<N>>();
}


template<typename... T>
template<size_t N>
constexpr const auto&
container<T...>::as() const noexcept
{
  return as<nth_type<N>>();
}


template<typename... T>
template<typename U>
auto&
container<T...>::get_cont() noexcept
{
  return this->template as<U>().cont;
}


template<typename... T>
template<typename U>
[[nodiscard]] const auto&
container<T...>::get_cont() const noexcept
{
  return this->template as<U>().cont;
}


template<typename... T>
template<typename... Elems>
void
container<T...>::push(const Elems&... elems)
{
  (this->template as<Elems>().cont.push_back(elems), ...);
}


template<typename... T>
template<typename... Elems>
void
container<T...>::push(Elems&&... elems)
{
  (this->template as<Elems>().cont.push_back(elems), ...);
}


template<typename... T>
template<typename U, typename... Args>
auto
container<T...>::emplace(Args&&... args) -> typename u_container<U>::reference
{
  return this->template as<U>().cont.emplace_back(args...);
}


template<typename... T>
container<T...>::const_iterator::const_iterator(
    const container<T...>& container) noexcept
    : cont_{&container}, curr_iter_{cont_->as<sizeof...(T) - 1>().cont.cend()}
{
}


template<typename... T>
container<T...>::const_iterator::const_iterator(
    const container<T...>& container,
    std::variant<u_const_iterator<T>...> iterator) noexcept
    : cont_{&container}, curr_iter_{iterator}
{
}


template<typename... T>
auto
container<T...>::const_iterator::operator*() const noexcept -> const_reference
{
  return std::visit(
      [](auto&& it) -> const_reference { return *it; }, curr_iter_);
}


template<typename... T>
bool
container<T...>::const_iterator::operator==(
    const const_iterator& rhs) const noexcept
{
  return curr_iter_ == rhs.curr_iter_;
}


template<typename... T>
bool
container<T...>::const_iterator::operator!=(
    const const_iterator& rhs) const noexcept
{
  return !(operator==(rhs));
}


template<typename... T>
auto
container<T...>::const_iterator::operator++() noexcept -> const_iterator&
{
  enum class state
  {
    NORMAL,
    INIT_NEXT,
    DONE,
  } state = state::NORMAL;

  auto advance = [this, &state]<typename U>(
                     bool holds_type_u, const auto& end, const auto& begin) {
    switch (state)
    {
    case state::NORMAL:
    {
      if (!holds_type_u)
        break;

      if (auto& unboxed = std::get<u_const_iterator<U>>(curr_iter_);
          unboxed != end)
      {
        ++unboxed;
        state = unboxed == end ? state::INIT_NEXT : state::DONE;
      }
      break;
    }

    case state::INIT_NEXT:
    {
      curr_iter_ = begin;

      // If there are no items of type T, we want the next type to be assigned.
      // Furthermore, if there are no more types with elements the final state
      // will be equal to end().
      if (begin != end)
        state = state::DONE;

      break;
    }

    case state::DONE: break;
    }
  };

  // TODO: This should be an operator fold instead of mutating global state...
  (advance.template operator()<T>(
       std::holds_alternative<u_const_iterator<T>>(curr_iter_),
       cont_->template get_cont<T>().end(),
       cont_->template get_cont<T>().begin()),
   ...);
  return *this;
}


template<typename... T>
auto
container<T...>::cbegin() const noexcept -> const_iterator
{
  enum class state
  {
    SEEKING,
    FOUND,
  } state = state::SEEKING;

  std::variant<u_const_iterator<T>...> begin_iter =
      this->template as<sizeof...(T) - 1>().cont.cend();

  auto seeker = [&state, &begin_iter]<typename U>(const auto& as) {
    if (state == state::SEEKING && !as.cont.empty())
    {
      begin_iter = as.cont.cbegin();
      state = state::FOUND;
    }
  };

  // TODO: This should be an operator fold instead of mutating global state...
  (seeker.template operator()<T>(this->template as<T>()), ...);
  return const_iterator(*this, begin_iter);
}


template<typename... T>
auto
container<T...>::cend() const noexcept -> const_iterator
{
  return const_iterator(*this);
}
} // namespace htracer::utils

#endif // HTRACER_UTILS_CONTAINER_HPP
