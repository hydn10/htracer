#ifndef HTRACER_RAYTRACING_SAMPLER_HPP
#define HTRACER_RAYTRACING_SAMPLER_HPP


#include <htracer/geometries/ray.hpp>
#include <htracer/numerics.hpp>
#include <htracer/raytracing/intersector.hpp>
#include <htracer/scene/scene.hpp>
#include <htracer/vector.hpp>


namespace htracer::raytracing
{

template<typename Float, template<typename> typename... Geometries>
colors::srgb_linear<Float>
sample(geometries::ray<Float> const &ray, scene::scene_view<scene::scene<Float, Geometries...>> scene);


namespace detail_
{

template<typename Float, template<typename> typename... Geometries>
colors::srgb_linear<Float>
sample(geometries::ray<Float> const &ray, scene::scene_view<scene::scene<Float, Geometries...>> scene, unsigned depth);

}


template<typename Float, template<typename> typename... Geometries>
colors::srgb_linear<Float>
sample(geometries::ray<Float> const &ray, scene::scene_view<scene::scene<Float, Geometries...>> scene)
{
  return detail_::sample(ray, scene, 0);
}


namespace detail_
{

template<typename Float, template<typename> typename... Geometries>
colors::srgb_linear<Float>
sample(geometries::ray<Float> const &ray, scene::scene_view<scene::scene<Float, Geometries...>> scene, unsigned depth)
{
  // TODO: Move to a sampler class and set as parameter toghether with MIN_DISTANCE.
  constexpr unsigned MAX_DEPTH = 5;

  // TODO: Small optimization: do not call if depth > MAX_DEPTH so ray is never created.
  if (depth > MAX_DEPTH)
    return {0., 0., 0.};

  // TODO: This should be a parameter since it is scale-dependant. The value
  // must be > 0 or else reflections/refractions wont work.
  constexpr Float MIN_DISTANCE = .002;

  auto const intersection = intersect(ray, scene, MIN_DISTANCE);

  if (!intersection)
    return {0., 0., 0.};

  auto const &[obj_dist, obj] = *intersection;

  auto const &material = obj.get_material();

  auto const p = ray.origin + obj_dist * ray.direction;
  auto const n = obj.get_geometry().normal(p);

  auto pixel_color = material.ambient_color;

  for (auto const &light : scene.lights())
  {
    auto const pl = light.position - p;
    auto const light_dist2 = dot(pl, pl);
    auto const light_dist = std::sqrt(light_dist2);
    auto const light_dist2_inv = 1 / light_dist2;
    auto const l = normalize(pl);

    auto const light_intersect = intersect({p, l}, scene, MIN_DISTANCE);

    if (light_intersect && light_intersect->first < light_dist)
      continue;

    auto const lambertian = std::max(dot(l, n), Float{0});

    pixel_color += (lambertian * light.intensity * light_dist2_inv) * material.diffuse_color;

    if (lambertian > 0) // TODO: Is this necessary? I suspect if lambertian == 0
                        // then specular == 0, so it is an optimization.
    {
      auto const h = normalize((l - ray.direction));
      auto const spec_angle = std::max(dot(h, n), Float{0});
      auto const specular = std::pow(spec_angle, material.shininess);

      pixel_color += (material.specular * specular * light.intensity * light_dist2_inv) * light.color;
    }
  }

  if (material.reflectivity > 0)
  {
    auto const reflect_dir = reflect(ray.direction, n);
    pixel_color += material.reflectivity * sample({p, reflect_dir}, scene, depth + 1);
  }

  // TODO: Should the color be saturated here or only just before being gamma
  // corrected (so recursive calls return unsaturated values)?
  return saturate(pixel_color);
}

} // namespace detail_
} // namespace htracer::raytracing

#endif
