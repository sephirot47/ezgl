#include "ez/RendererRayTracingCPU.h"

namespace ez
{
template <RendererRayTracingCPU::EStateId StateId>
void RendererRayTracingCPU::ApplyState(const State::ValueType<StateId>& inValue, State& ioState)
{
  if constexpr (StateId == RendererRayTracingCPU::EStateId::CAMERA) {}
  else
  {
    static_assert((static_cast<int>(StateId) == -1), "ApplyState not implemented for this state id.");
  }
}

template <RendererRayTracingCPU::EStateId StateId>
typename RendererRayTracingCPU::State::template ValueType<StateId> RendererRayTracingCPU::GetDefaultValue()
{
  if constexpr (StateId == RendererRayTracingCPU::EStateId::CAMERA)
  {
    return std::make_shared<PerspectiveCameraf>();
  }
  else
  {
    static_assert((static_cast<int>(StateId) == -1), "GetDefaultValue not implemented for this state id.");
  }
  return {};
}

namespace internal
{
  Vec3f LocalNormal(const ObjectRayTracing& inObject, const Vec3f& inIntersectionPoint)
  {
    switch (inObject.GetType())
    {
    case ObjectRayTracing::ETypeId::SPHERE:
    {
      const auto& sphere = inObject.GetObject<Spheref>();
      return NormalizedSafe(inIntersectionPoint - sphere.GetCenter());
    }
    break;

    case ObjectRayTracing::ETypeId::AABOX:
    {
      const auto& aabox = inObject.GetObject<AABoxf>();
      const auto vector_from_center = (inIntersectionPoint - aabox.GetCenter());
      const auto vector_from_center_from_0_to_1 = (vector_from_center / aabox.GetSize());

      const auto max_index = MaxIndex(Abs(vector_from_center_from_0_to_1));
      auto vector_from_center_from_0_to_1_all_0s_but_1_in_the_max_component = Zero<Vec3f>();
      vector_from_center_from_0_to_1_all_0s_but_1_in_the_max_component[max_index]
          = Sign(vector_from_center_from_0_to_1[max_index]);
      return vector_from_center_from_0_to_1_all_0s_but_1_in_the_max_component;
    }
    break;

    default:
      assert(!"This should never be called!");
      break;
    }
    return Zero<Vec3f>();
  }
}
template <EIntersectMode TIntersectMode>
auto RendererRayTracingCPU::RayTraceInScene(const SceneElements& inSceneElements,
    const Ray3f& inRay,
    const std::size_t inCurrentBounces)
{
  static_assert(TIntersectMode == EIntersectMode::ONLY_CHECK || TIntersectMode == EIntersectMode::ONLY_CLOSEST,
      "Unsupported EIntersectMode.");

  if constexpr (TIntersectMode == EIntersectMode::ONLY_CLOSEST)
  {
    if (inCurrentBounces >= 2)
      return std::optional<IntersectionResult> {};
  }

  // Get intersection of the ray with all the AABBoxes of objects, using the world space AABBox octree,
  // and sort it from closest to furthest
  auto world_aabbox_intersections = IntersectAll(inSceneElements.mDrawableWorldAABBoxesOctree, inRay);
  if constexpr (TIntersectMode == EIntersectMode::ONLY_CLOSEST)
  {
    std::sort(world_aabbox_intersections.begin(),
        world_aabbox_intersections.end(),
        [](const auto& inLHS, const auto& inRHS) { return inLHS.mDistance < inRHS.mDistance; });
  }

  // Now, for each AABBox intersected, test intersection with the object itself
  // (which can be a geometric primitive or another octree of a mesh for example)
  const ObjectRayTracing* closest_intersected_drawable = nullptr;
  auto closest_intersected_transformation_matrix = Mat4f {};
  auto closest_intersection_world_sq_distance = Max<float>();
  for (const auto& world_aabbox_intersection : world_aabbox_intersections)
  {
    // We have intersected a object AABBox, now cast against the object itself
    const auto& intersected_drawable_index = world_aabbox_intersection.mPrimitiveIndex;
    const auto& intersected_drawable_in_space = inSceneElements.mDrawables.at(intersected_drawable_index);
    const auto& intersected_drawable_transformation = intersected_drawable_in_space.mWorldTransformation;
    const auto intersected_drawable = intersected_drawable_in_space.mDrawable;
    const auto local_ray = InverseTransformed(inRay, intersected_drawable_transformation); // Ray in object/local space
    const auto drawable_intersection_distance_opt = IntersectClosest(*intersected_drawable, local_ray);
    if (!drawable_intersection_distance_opt)
      continue;

    if constexpr (TIntersectMode == EIntersectMode::ONLY_CHECK)
    {
      return true;
    }

    const auto& drawable_intersection_local_distance = *drawable_intersection_distance_opt;
    const auto drawable_intersection_local_point = local_ray.GetPoint(drawable_intersection_local_distance);
    const auto drawable_intersection_world_point
        = Transformed(drawable_intersection_local_point, intersected_drawable_transformation);
    const auto drawable_intersection_world_sq_distance
        = SqDistance(drawable_intersection_world_point, inRay.GetOrigin());
    if (drawable_intersection_world_sq_distance > closest_intersection_world_sq_distance)
      continue;

    closest_intersection_world_sq_distance = drawable_intersection_world_sq_distance;
    closest_intersected_transformation_matrix = intersected_drawable_in_space.mWorldTransformation;
    closest_intersected_drawable = intersected_drawable;
  }

  if constexpr (TIntersectMode == EIntersectMode::ONLY_CHECK)
  {
    return false;
  }
  else if constexpr (TIntersectMode == EIntersectMode::ONLY_CLOSEST)
  {
    if (!closest_intersected_drawable)
      return std::optional<IntersectionResult> {};
  }

  const auto& transformation_matrix = closest_intersected_transformation_matrix;
  const auto transformation_normal_matrix = Transposed(Inverted(transformation_matrix));

  const auto& material = closest_intersected_drawable->GetMaterial();
  const auto material_color = material.GetColor();
  const auto intersection_distance = Sqrt(closest_intersection_world_sq_distance);

  auto surface_color = XYZ(material_color) * All<Color3f>(0.3f);
  const auto surface_point = inRay.GetPoint(intersection_distance);
  const auto surface_local_point = InverseTransformed(surface_point, transformation_matrix);
  const auto surface_local_normal = internal::LocalNormal(*closest_intersected_drawable, surface_local_point);
  const auto surface_normal = NormalizedSafe(Transformed(surface_local_normal, transformation_normal_matrix));
  for (const auto& directional_light : inSceneElements.mDirectionalLights)
  {
    constexpr auto Epsilon = 1e-3f;
    const auto reflected_ray_direction = material.GetReflectDirection(inRay.GetDirection(), surface_normal);
    const auto reflected_ray_origin = surface_point + (reflected_ray_direction * Epsilon);
    /*
    const auto reflected_ray = Ray3f { reflected_ray_origin, reflected_ray_direction };
    const auto reflected_ray_intersection = RayTraceInScene(inSceneElements, reflected_ray, inCurrentBounces + 1);
    */
    const auto light_inv_ray_direction = -directional_light.mDirection;
    EXPECTS(IsNormalized(light_inv_ray_direction));
    const auto light_inv_ray_origin
        = surface_point + (light_inv_ray_direction * Epsilon); //  + (surface_normal * Epsilon);
    const auto light_inv_ray = Ray3f { light_inv_ray_origin, light_inv_ray_direction };
    const auto is_in_shadow
        = RayTraceInScene<EIntersectMode::ONLY_CHECK>(inSceneElements, light_inv_ray, inCurrentBounces + 1);
    if (is_in_shadow)
      continue;

    const auto& directional_light_color = directional_light.mColor;
    const auto light_dot = Max(Dot(-directional_light.mDirection, reflected_ray_direction), 0.0f);
    const auto specular_incidence = light_dot;
    const auto diffuse_incidence = (1.0f - specular_incidence) * light_dot;

    const auto specular = specular_incidence * directional_light_color;
    const auto diffuse = diffuse_incidence * directional_light_color * XYZ(material_color);
    const auto light = (specular + diffuse);
    surface_color += light;
  }

  if constexpr (TIntersectMode == EIntersectMode::ONLY_CLOSEST)
  {
    IntersectionResult final_intersection_result;
    final_intersection_result.mDistance = intersection_distance;
    final_intersection_result.mColor = surface_color;
    return std::make_optional(final_intersection_result);
  }
  else if constexpr (TIntersectMode == EIntersectMode::ONLY_CHECK)
  {
    assert(!"Should never get here");
    return false;
  }
}
}
