#ifndef HTRACER_UTILS_HETEROGENEUS_VISITABLE_HPP
#define HTRACER_UTILS_HETEROGENEUS_VISITABLE_HPP


#include <tuple>
#include <vector>


namespace htracer::utils
{

template<typename... Ts>
class heterogeneous_visitable
{
  std::tuple<std::vector<Ts>...> vectors_;

public:
  template<typename T>
  // TODO: concept restrictions!
  constexpr void
  push(T &&item);
  template<typename T, typename... Args>
  // TODO: concept restrictions!
  constexpr void
  emplace(Args... args);

  template<typename F>
  constexpr void
  visit(F &&f) const;
  template<typename F>
  constexpr void
  visit(F &&f);
};


template<typename... Ts>
template<typename T>
constexpr void
heterogeneous_visitable<Ts...>::push(T &&item)
{
  std::get<std::vector<T>>(vectors_).push_back(std::forward<T>(item));
}


template<typename... Ts>
template<typename T, typename... Args>
constexpr void
heterogeneous_visitable<Ts...>::emplace(Args... args)
{
  std::get<std::vector<T>>(vectors_).emplace_back(std::forward<Args>(args)...);
}


template<typename... Ts>
template<typename F>
constexpr void
heterogeneous_visitable<Ts...>::visit(F &&f) const
{
  auto apply_f_to_vec = [&f](auto const &vec)
  {
    for (auto const &item : vec)
      f(item);
  };

  (apply_f_to_vec(std::get<std::vector<Ts>>(vectors_)), ...);
}


template<typename... Ts>
template<typename F>
constexpr void
heterogeneous_visitable<Ts...>::visit(F &&f)
{
  auto apply_f_to_vec = [&f](auto const &vec)
  {
    for (auto const &item : vec)
      f(item);
  };

  (apply_f_to_vec(std::get<std::vector<Ts>>(vectors_)), ...);
}

} // namespace htracer::utils

#endif