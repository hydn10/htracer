#ifndef HTRACER_SCENE_SCENE_VIEW_HPP
#define HTRACER_SCENE_SCENE_VIEW_HPP


#include <htracer/scene/scene.hpp>


namespace htracer::scene
{

template<typename Scene>
class scene_view
{
  std::reference_wrapper<const Scene> scene_wrp_;

public:
  scene_view(Scene const &scene);

  template<typename F>
  void
  for_each_object(F &&f) const;

  constexpr auto const &
  lights() const;
};


template<typename Scene>
scene_view<Scene>::scene_view(Scene const &scene)
    : scene_wrp_{scene}
{
}


template<typename Scene>
template<typename F>
void
scene_view<Scene>::for_each_object(F &&f) const
{
  scene_wrp_.get().for_each_object(std::forward<F>(f));
}


template<typename Scene>
constexpr auto const &
scene_view<Scene>::lights() const
{
  return scene_wrp_.get().lights();
}

} // namespace htracer::scene

#endif