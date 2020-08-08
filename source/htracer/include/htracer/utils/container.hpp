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


template<typename T>
class single_vector
{
public:
  std::vector<T> vec;

  using reference = typename decltype(vec)::reference;
};


template<typename T>
class single_iterator
{
public:
  typename std::vector<T>::iterator iter;
};
} // namespace detail_


template<typename... T>
class container : private detail_::single_vector<T>...
{
  template<std::size_t N>
  using nth_type = typename detail_::type_at<N, T...>::type;

  template<std::size_t N>
  using nth_vector = std::vector<nth_type<N>>;

  template<std::size_t N>
  using nth_vector_iterator = typename nth_vector<N>::iterator;

  template<std::size_t N>
  using nth_vector_const_iterator = typename nth_vector<N>::const_iterator;

  template<typename U>
  constexpr auto&
  as();

  template<typename U>
  [[nodiscard]] constexpr const auto&
  as() const;

  template<size_t N>
  constexpr auto&
  as();

  template<size_t N>
  [[nodiscard]] constexpr const auto&
  as() const;

  template<typename U>
  auto&
  get_vec();

  template<typename U>
  [[nodiscard]] const auto&
  get_vec() const;

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
  typename detail_::single_vector<U>::reference
  emplace(Args&&... args);

  class const_iterator
  {
    friend class container<T...>;

    const container<T...>* cont_;
    std::variant<typename std::vector<T>::const_iterator...> curr_iter_;

    explicit const_iterator(const container<T...>& container);
    const_iterator(
        const container<T...>& container,
        std::variant<typename std::vector<T>::const_iterator...> iterator);

  public:
    const_reference
    operator*();
    bool
    operator==(const const_iterator& rhs) const;
    bool
    operator!=(const const_iterator& rhs) const;
    const_iterator&
    operator++();
  };

  const_iterator
  cbegin() const;
  const_iterator
  cend() const;
};


template<typename... T>
template<typename U>
constexpr auto&
container<T...>::as()
{
  return static_cast<detail_::single_vector<U>&>(*this);
}


template<typename... T>
template<typename U>
[[nodiscard]] constexpr const auto&
container<T...>::as() const
{
  return static_cast<const detail_::single_vector<U>&>(*this);
}


template<typename... T>
template<size_t N>
constexpr auto&
container<T...>::as()
{
  return as<nth_type<N>>();
}


template<typename... T>
template<size_t N>
constexpr const auto&
container<T...>::as() const
{
  return as<nth_type<N>>();
}


template<typename... T>
template<typename U>
auto&
container<T...>::get_vec()
{
  return this->template as<U>().vec;
}


template<typename... T>
template<typename U>
[[nodiscard]] const auto&
container<T...>::get_vec() const
{
  return this->template as<U>().vec;
}


template<typename... T>
template<typename... Elems>
void
container<T...>::push(const Elems&... elems)
{
  (this->template as<Elems>().vec.push_back(elems), ...);
}


template<typename... T>
template<typename... Elems>
void
container<T...>::push(Elems&&... elems)
{
  (this->template as<Elems>().vec.push_back(elems), ...);
}


template<typename... T>
template<typename U, typename... Args>
typename detail_::single_vector<U>::reference
container<T...>::emplace(Args&&... args)
{
  return this->template as<U>().vec.emplace_back(args...);
}


template<typename... T>
container<T...>::const_iterator::const_iterator(
    const container<T...>& container)
    : cont_{&container}, curr_iter_{cont_->as<sizeof...(T) - 1>().vec.cend()}
{
}


template<typename... T>
container<T...>::const_iterator::const_iterator(
    const container<T...>& container,
    std::variant<typename std::vector<T>::const_iterator...> iterator)
    : cont_{&container}, curr_iter_{iterator}
{
}


template<typename... T>
auto
container<T...>::const_iterator::operator*() -> const_reference
{
  return std::visit(
      [](auto&& it) -> const_reference { return *it; }, curr_iter_);
}


template<typename... T>
bool
container<T...>::const_iterator::operator==(const const_iterator& rhs) const
{
  return curr_iter_ == rhs.curr_iter_;
}


template<typename... T>
bool
container<T...>::const_iterator::operator!=(const const_iterator& rhs) const
{
  return !(operator==(rhs));
}


template<typename... T>
auto
container<T...>::const_iterator::operator++() -> const_iterator&
{
  enum class state
  {
    NORMAL,
    INIT_NEXT,
    DONE,
  } state = state::NORMAL;

  auto advance = [this, &state]<typename U>(
                     bool holds, const auto& end, const auto& begin) {
    switch (state)
    {
    case state::NORMAL:
    {
      if (!holds)
        break;

      if (auto& unboxed = std::get<typename std::vector<U>::const_iterator>(
              this->curr_iter_);
          unboxed != end)
      {
        ++unboxed;
        state = unboxed == end ? state::INIT_NEXT : state::DONE;
      }
      break;
    }

    case state::INIT_NEXT:
    {
      this->curr_iter_ = begin;

      // Edge case. If there are no items of type T, we want the next type to
      // be assigned, so if there are no more types with elements the final
      // state will be equal to end().
      if (begin != end)
        state = state::DONE;

      break;
    }

    case state::DONE: break;
    }
  };

  (advance.template operator()<T>(
       std::holds_alternative<typename std::vector<T>::const_iterator>(
           this->curr_iter_),
       this->cont_->template get_vec<T>().end(),
       this->cont_->template get_vec<T>().begin()),
   ...);
  return *this;
}


template<typename... T>
auto
container<T...>::cbegin() const -> const_iterator
{
  enum class state
  {
    SEEKING,
    FOUND,
  } state = state::SEEKING;

  std::variant<typename std::vector<T>::const_iterator...> begin_iter =
      this->template as<sizeof...(T) - 1>().vec.cend();

  auto seeker = [&state, &begin_iter]<typename U>(const auto& as) {
    if (state == state::SEEKING && !as.vec.empty())
    {
      begin_iter = as.vec.cbegin();
      state = state::FOUND;
    }
  };

  (seeker.template operator()<T>(this->template as<T>()), ...);
  return const_iterator(*this, begin_iter);
}


template<typename... T>
auto
container<T...>::cend() const -> const_iterator
{
  return const_iterator(*this);
}
} // namespace htracer::utils

#endif // HTRACER_UTILS_CONTAINER_HPP
