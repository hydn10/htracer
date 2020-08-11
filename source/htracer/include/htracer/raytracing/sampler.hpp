#ifndef HTRACER_RAYTRACING_SAMPLER_HPP
#define HTRACER_RAYTRACING_SAMPLER_HPP


#include <htracer/color.hpp>
#include <htracer/geometry/ray.hpp>
#include <htracer/numerics.hpp>
#include <htracer/raytracing/intersector.hpp>
#include <htracer/scene/scene.hpp>
#include <htracer/vector.hpp>


namespace htracer::raytracing
{
template<typename Float>
v3<Float>
sample(geometry::ray<Float> ray, const scene::scene<Float>& scene);

namespace detail_
{
template<typename Float>
v3<Float>
sample(
    geometry::ray<Float> ray,
    const scene::scene<Float>& scene,
    unsigned depth);
}


template<typename Float>
v3<Float>
sample(geometry::ray<Float> ray, const scene::scene<Float>& scene)
{
  const auto final_color = detail_::sample(ray, scene, 0);

  // After reading a lot about gamma, I still don't know if I am supposed to
  // gamma correct the output or not.
  // Probably experiment more and see what I like the best.
  // Take in account that this will change input color values if they are
  // hardcoded. They must be raised to the 1/2.2 power (or viceversa?) if they
  // are to be compared.
  return pow(final_color, 1 / 2.2);
}


namespace detail_
{
template<typename Float>
v3<Float>
sample(
    geometry::ray<Float> ray,
    const scene::scene<Float>& scene,
    unsigned depth)
{
  constexpr unsigned MAX_DEPTH = 5;

  if (depth > MAX_DEPTH)
    return {0., 0., 0.};

  // TODO: This should be a parameter since it is scale-dependant. The value
  // must be > 0 or else reflections/refractions wont work.
  constexpr Float MIN_DISTANCE = .02;

  const auto intersection = intersect(ray, scene.objects(), MIN_DISTANCE);

  if (!intersection)
    return {0., 0., 0.};

  auto const& [obj_dist, obj_iter] = *intersection;

  auto const& material = std::visit(
      [](const auto& v) -> scene::material<Float> const& {
        return v.get().mat;
      },
      *obj_iter);

  const auto p = ray.origin + obj_dist * ray.direction;
  const auto n = std::visit(
      [&p](const auto& v) { return v.get().geometry.normal(p); }, *obj_iter);

  auto pixel_color = material.ambient_color;

  for (const auto& light: scene.lights())
  {
    const auto pl = light.position - p;
    const auto light_dist2 = dot(pl, pl);
    const auto light_dist = std::sqrt(light_dist2);
    const auto light_dist2_inv = 1 / light_dist2;
    const auto l = pl.normalized();

    const auto light_intersect =
        intersect({p, l}, scene.objects(), MIN_DISTANCE);

    if (light_intersect && light_intersect->first < light_dist)
      continue;

    const auto lambertian = std::max(dot(l, n), Float{0});

    pixel_color +=
        material.diffuse_color * lambertian * light.intensity * light_dist2_inv;

    if (lambertian > 0) // TODO: Is this necessary? I suspect if lambertian == 0
                        // then specular == 0, so it is an optimization.
    {
      const auto h = (l - ray.direction).normalized();
      const auto spec_angle = std::max(dot(h, n), Float{0});
      const auto specular = std::pow(spec_angle, material.shininess);

      pixel_color += light.color * material.specular * specular
                     * light.intensity * light_dist2_inv;
    }
  }

  if (material.reflectivity > 0)
  {
    const auto reflect_dir = reflect(ray.direction, n);
    const auto reflect_color =
        material.reflectivity * sample({p, reflect_dir}, scene, depth + 1);

    pixel_color += reflect_color;
  }

  // TODO: Should the color be saturated here or only just before being gamma
  // corrected (so recursive calls return unsaturated values)?
  return saturate(pixel_color);
}

} // namespace detail_
} // namespace htracer::raytracing

#endif // HTRACER_RAYTRACING_SAMPLER_HPP
