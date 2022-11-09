#ifndef HTRACER_SCENE_SCENE_VIEW_HPP
#define HTRACER_SCENE_SCENE_VIEW_HPP


#include <htracer/scene/scene.hpp>


namespace htracer::scene
{

template<typename Scene>
class scene_view
{
  Scene const &scene_;

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
    : scene_{scene}
{
}


template<typename Scene>
template<typename F>
void
scene_view<Scene>::for_each_object(F &&f) const
{
  scene_.for_each_object(std::forward<F>(f));
}


template<typename Scene>
constexpr auto const &
scene_view<Scene>::lights() const
{
  return scene_.lights();
}

} // namespace htracer::scene

#endif