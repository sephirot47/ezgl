#include "ez/RendererRayTracingCPU.h"
#include "ez/Quat.h"
#include <queue>

namespace ez
{
RendererRayTracingCPU::RendererRayTracingCPU()
{
  mRenderTarget.Create(1, 1);
  PushAllDefaultStateValues();
}

void RendererRayTracingCPU::Clear(const Color4f& inClearColor) { mRenderTarget.Fill(inClearColor); }

void RendererRayTracingCPU::SetSize(const Vec2ul& inSize)
{
  EXPECTS(inSize >= One<Vec2ul>());

  mRenderTarget.Create(inSize[0], inSize[1]);
  Clear(Black<Color4f>());
}

Vec2ul RendererRayTracingCPU::GetSize() const { return Vec2ul { mRenderTarget.GetWidth(), mRenderTarget.GetHeight() }; }

void RendererRayTracingCPU::AdaptCameraAspectRatio()
{
  if (const auto perspective_camera = GetPerspectiveCamera())
  {
    perspective_camera->SetAspectRatio(static_cast<float>(GetSize()[0]) / GetSize()[1]);
  }
}

void RendererRayTracingCPU::SetCamera(const std::shared_ptr<Camera3f>& inCamera)
{
  mState.GetCurrent<RendererRayTracingCPU::EStateId::CAMERA>() = inCamera;
}

std::shared_ptr<Camera3f> RendererRayTracingCPU::GetCamera()
{
  return mState.GetCurrent<RendererRayTracingCPU::EStateId::CAMERA>();
}
std::shared_ptr<const Camera3f> RendererRayTracingCPU::GetCamera() const
{
  return const_cast<RendererRayTracingCPU&>(*this).GetCamera();
}

void RendererRayTracingCPU::PushState() { mState.PushAllTops(); }

void RendererRayTracingCPU::PopState() { mState.PopAll(); }

void RendererRayTracingCPU::ResetState()
{
  PopState();
  PushAllDefaultStateValues();
}

void RendererRayTracingCPU::PushAllDefaultStateValues() { mState.PushAllDefaultValues(); }

std::shared_ptr<PerspectiveCameraf> RendererRayTracingCPU::GetPerspectiveCamera()
{
  return std::dynamic_pointer_cast<PerspectiveCameraf>(mState.GetCurrent<RendererRayTracingCPU::EStateId::CAMERA>());
}

std::shared_ptr<const PerspectiveCameraf> RendererRayTracingCPU::GetPerspectiveCamera() const
{
  return const_cast<RendererRayTracingCPU&>(*this).GetPerspectiveCamera();
}

std::shared_ptr<OrthographicCamera3f> RendererRayTracingCPU::GetOrthographicCamera()
{
  return std::dynamic_pointer_cast<OrthographicCamera3f>(mState.GetCurrent<RendererRayTracingCPU::EStateId::CAMERA>());
}

std::shared_ptr<const OrthographicCamera3f> RendererRayTracingCPU::GetOrthographicCamera() const
{
  return const_cast<RendererRayTracingCPU&>(*this).GetOrthographicCamera();
}

std::optional<Color4f> RendererRayTracingCPU::RayTraceInScene(const Octree<DrawableRayTracingWrapper>& inSceneOctree,
    const std::vector<DrawableRayTracingWrapper>& inSceneDrawables,
    const Ray3f& inRay)
{
  UNUSED(inSceneOctree);
  UNUSED(inSceneDrawables);

  //*
  // Get intersection of the ray with all the AABBoxes of drawables, using the world space AABBox octree,
  // and sort it from closest to furthest
  auto world_aabbox_intersections = IntersectAll(inSceneOctree, inRay);
  std::sort(world_aabbox_intersections.begin(),
      world_aabbox_intersections.end(),
      [](const auto& inLHS, const auto& inRHS) { return inLHS.mDistance < inRHS.mDistance; });

  // Now, for each AABBox intersected, test intersection with the drawable itself (which can be a geometric primitive or
  // another octree of a mesh for example)
  auto closest_intersection_color = std::optional<Color4f>();
  auto closest_intersection_distance = Max<float>();
  for (const auto& world_aabbox_intersection : world_aabbox_intersections)
  {
    // We have intersected a drawable AABBox, now cast against the drawable itself
    const auto& intersected_drawable_index = world_aabbox_intersection.mPrimitiveIndex;
    const auto& intersected_drawable_wrapper = inSceneDrawables.at(intersected_drawable_index);
    const auto& intersected_drawable = *intersected_drawable_wrapper.mDrawable;
    const auto& intersected_drawable_transformation = intersected_drawable_wrapper.mWorldTransformation;
    const auto local_ray
        = InverseTransformed(inRay, intersected_drawable_transformation); // Ray in drawable/local space
    const auto drawable_intersection_distance_opt = IntersectClosest(intersected_drawable, local_ray);
    if (!drawable_intersection_distance_opt)
      continue;

    const auto& drawable_intersection_distance = *drawable_intersection_distance_opt;
    if (drawable_intersection_distance > closest_intersection_distance)
      continue;

    closest_intersection_distance = drawable_intersection_distance;

    const auto& material = intersected_drawable.GetMaterial();
    closest_intersection_color = material.GetColor();
  }

  return closest_intersection_color;
  /*/

  // Below: without scene ABBox Octree for testing purposes
  UNUSED(inSceneOctree);
  for (const auto& drawable_wrapper : inSceneDrawables)
  {
    const auto local_ray = InverseTransformed(inRay, drawable_wrapper.mWorldTransformation);
    const auto drawable_intersection_result = IntersectClosest(*drawable_wrapper.mDrawable, local_ray);
    if (drawable_intersection_result)
      return Red<Color4f>();
  }
  return Zero<Color4f>();
  //*/
}

void RendererRayTracingCPU::DrawScene(const SceneGraphNode<DrawableRayTracing>& inScene)
{
  // Calculate scene octree,
  Octree<DrawableRayTracingWrapper> scene_octree;
  std::vector<DrawableRayTracingWrapper> scene_drawables;
  CreateSceneOctree(inScene, scene_octree, scene_drawables);

  // RayTrace each pixel
  RayTraceFullRenderTarget([&](const auto& inRay) {
    const auto hit_color = RayTraceInScene(scene_octree, scene_drawables, inRay);
    return hit_color.value_or(Zero<Color4f>());
  });
}

void RendererRayTracingCPU::RayTraceFullRenderTarget(const std::function<Color4f(const Ray3f&)>& inRayTraceFunction)
{
  EXPECTS(GetPerspectiveCamera());
  const auto& cam = *GetPerspectiveCamera();

  const auto viewport_size = Vec2f { GetSize() };
  const auto world_ray_origin = cam.GetPosition();

  const auto z_near = cam.GetZNear();
  const auto half_angle_of_view = cam.GetFullAngleOfView() * 0.5f;
  const auto render_target_size = Vec2f { mRenderTarget.GetWidth(), mRenderTarget.GetHeight() };
  const auto advance = All<Vec2f>(2.0f) / render_target_size;
  const auto half_angle_of_view_tan = All<Vec2f>(Tan(half_angle_of_view)) * Vec2f { cam.GetAspectRatio(), 1.0f };
  const auto z_neared_half_angle_of_view_tan = z_near * half_angle_of_view_tan;

  auto y_ndc = -1.0f;
  for (std::size_t y = 0; y < mRenderTarget.GetHeight(); ++y)
  {
    const auto ray_dir_y = z_neared_half_angle_of_view_tan[1] * y_ndc;

    auto x_ndc = -1.0f;
    for (std::size_t x = 0; x < mRenderTarget.GetWidth(); ++x)
    {
      const auto ray_dir_x = z_neared_half_angle_of_view_tan[0] * x_ndc;
      const auto view_ray_direction = Normalized(Vec3f { ray_dir_x, ray_dir_y, -z_near });
      const auto world_ray_direction = (cam.GetRotation() * view_ray_direction);
      const auto world_ray = Ray3f { world_ray_origin, world_ray_direction };

      const auto pixel_color = inRayTraceFunction(world_ray);
      mRenderTarget.Get(x, y) = pixel_color;

      x_ndc += advance[0];
    }
    y_ndc += advance[1];
  }
}

void RendererRayTracingCPU::CreateSceneOctree(const SceneGraphNode<DrawableRayTracing>& inScene,
    Octree<DrawableRayTracingWrapper>& outOctree,
    std::vector<DrawableRayTracingWrapper>& outDrawables) const
{
  std::queue<const SceneGraphNode<DrawableRayTracing>*> nodes_to_process;
  nodes_to_process.push(&inScene);
  while (!nodes_to_process.empty())
  {
    const auto node = nodes_to_process.front();
    nodes_to_process.pop();

    if (const auto drawable = node->GetObject())
    {
      const auto world_transformation = node->GetTransformation();
      const auto local_bounding_aabox = BoundingAABox(*drawable);
      const auto world_bounding_aabox = BoundingAABoxTransformed(local_bounding_aabox, world_transformation);
      outDrawables.emplace_back(drawable, world_transformation, world_bounding_aabox);
    }

    for (const auto& child_node : *node) nodes_to_process.push(child_node.get());
  }

  constexpr auto MaxPrimitivesAtLeafNodes = 1;
  constexpr auto MaxLevels = 5; // There needs to be a max level, because bboxes can be inter-contained
  outOctree = Octree<DrawableRayTracingWrapper> { MakeSpan(outDrawables), MaxPrimitivesAtLeafNodes, MaxLevels };
}

}