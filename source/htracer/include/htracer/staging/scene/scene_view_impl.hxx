#ifndef HTRACER_STAGING_SCENE_SCENE_VIEW_IMPL_HXX
#define HTRACER_STAGING_SCENE_SCENE_VIEW_IMPL_HXX


#include <htracer/staging/scene/scene.hxx>
#include <htracer/staging/scene/scene_view.hxx>


namespace htracer::staging
{

template<typename Float, template<typename> typename... Geometries>
scene_view<Float, Geometries...>::scene_view(scene<Float, Geometries...> const &scene) noexcept
    : scene_{scene}
{
}


template<typename Float, template<typename> typename... Geometries>
template<typename F>
void
scene_view<Float, Geometries...>::for_each_object(F &&f) const
{
  scene_.for_each_object(std::forward<F>(f));
}


template<typename Float, template<typename> typename... Geometries>
constexpr auto const &
scene_view<Float, Geometries...>::lights() const
{
  return scene_.lights();
}

} // namespace htracer::staging

#endif
