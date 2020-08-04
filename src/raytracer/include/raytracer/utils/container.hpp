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


template<typename T>
struct dereference_proxy
{
  std::optional<T> opt;
};


template<typename T>
dereference_proxy<T>
operator^(const dereference_proxy<T>& a, const dereference_proxy<T>& b);
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

  template<typename U>
  constexpr auto&
  as();

  template<typename U>
  [[nodiscard]] constexpr const auto&
  as() const;

  template<size_t N>
  auto&
  as();

  template<typename U>
  auto&
  get_vec();

  template<typename U>
  [[nodiscard]] const auto&
  get_vec() const;

public:
  using reference = std::variant<std::reference_wrapper<T>...>;

  template<typename... Elems>
  void
  push(const Elems&... elems);

  template<typename... Elems>
  void
  push(Elems&&... elems);

  template<typename U, typename... Args>
  typename detail_::single_vector<U>::reference
  emplace(Args&&... args);

  class iterator
  {
    friend class container<T...>;

    container<T...>& cont_;
    std::variant<typename std::vector<T>::iterator...> curr_iter_;

    explicit iterator(container<T...>& container);
    iterator(
        container<T...>& container,
        nth_vector_iterator<0> first_vec_iterator);

  public:
    reference
    operator*();
    bool
    operator==(const iterator& rhs) const;
    bool
    operator!=(const iterator& rhs) const;
    iterator&
    operator++();
  };

  iterator
  begin();
  iterator
  end();
};


namespace detail_
{
template<typename T>
dereference_proxy<T>
operator^(const dereference_proxy<T>& a, const dereference_proxy<T>& b)
{
  return a.opt ? a : b;
}
} // namespace detail_


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
auto&
container<T...>::as()
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
container<T...>::iterator::iterator(container<T...>& container)
    : cont_{container}, curr_iter_{cont_.as<sizeof...(T) - 1>().vec.end()}
{
}


template<typename... T>
container<T...>::iterator::iterator(
    container<T...>& container,
    nth_vector_iterator<0> first_vec_iterator)
    : cont_{container}, curr_iter_{first_vec_iterator}
{
}


template<typename... T>
auto
container<T...>::iterator::operator*() -> reference
{
  return std::visit([](auto&& it) -> reference { return *it; }, curr_iter_);
}


template<typename... T>
bool
container<T...>::iterator::operator==(const iterator& rhs) const
{
  return curr_iter_ == rhs.curr_iter_;
}


template<typename... T>
bool
container<T...>::iterator::operator!=(const iterator& rhs) const
{
  return !(operator==(rhs));
}


template<typename... T>
auto
container<T...>::iterator::operator++() -> iterator&
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

      if (auto& unboxed =
              std::get<typename std::vector<U>::iterator>(this->curr_iter_);
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
       std::holds_alternative<typename std::vector<T>::iterator>(
           this->curr_iter_),
       this->cont_.template get_vec<T>().end(),
       this->cont_.template get_vec<T>().begin()),
   ...);
  return *this;
}


template<typename... T>
auto
container<T...>::begin() -> iterator
{
  auto first_vec_begin = this->template as<0>().vec.begin();
  return iterator(*this, first_vec_begin);
}


template<typename... T>
auto
container<T...>::end() -> iterator
{
  return iterator(*this);
}
} // namespace htracer::utils

#endif // HTRACER_UTILS_CONTAINER_HPP
