#ifndef HTRACER_STAGING_SCENE_SCENE_VIEW_HXX
#define HTRACER_STAGING_SCENE_SCENE_VIEW_HXX


namespace htracer::staging
{

template<typename Float, template<typename> typename... Geometries>
class scene;


template<typename Float, template<typename> typename... Geometries>
class scene_view
{
  scene<Float, Geometries...> const &scene_;

public:
  explicit scene_view(scene<Float, Geometries...> const &scene) noexcept;

  template<typename F>
  void
  for_each_object(F &&f) const;

  constexpr auto const &
  lights() const;
};

} // namespace htracer::staging

#endif
