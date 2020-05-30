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

void RendererRayTracingCPU::DrawScene(const SceneGraphNode<ObjectRayTracing>& inScene)
{
  // Calculate scene elements,
  const auto scene_elements = CreateSceneElements(inScene);

  // RayTrace each pixel
  RayTraceFullRenderTarget([&](const auto& inRay) {
    const auto intersection_result = RayTraceInScene<EIntersectMode::ONLY_CLOSEST>(scene_elements, inRay, 0);
    if (!intersection_result)
      return Zero<Color4f>();
    return XYZ1(intersection_result->mColor);
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
    PRINT(y << "/" << mRenderTarget.GetHeight());

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

RendererRayTracingCPU::SceneElements RendererRayTracingCPU::CreateSceneElements(
    const SceneGraphNode<ObjectRayTracing>& inScene) const
{
  SceneElements scene_elements;

  std::queue<const SceneGraphNode<ObjectRayTracing>*> nodes_to_process;
  nodes_to_process.push(&inScene);
  while (!nodes_to_process.empty())
  {
    const auto node = nodes_to_process.front();
    nodes_to_process.pop();

    for (const auto& child_node : *node) nodes_to_process.push(child_node.get());

    const auto object = node->GetObject();
    if (!object)
      continue;

    const auto world_transformation_matrix = node->GetWorldTransformationMatrix();
    if (object->IsDrawable())
    {
      const auto local_bounding_aabox = BoundingAABox(*object);
      const auto world_bounding_aabox = Transformed(local_bounding_aabox, world_transformation_matrix);
      scene_elements.mDrawables.emplace_back(object, world_transformation_matrix, world_bounding_aabox);
    }
    else if (object->IsLight())
    {
      auto world_light = object->GetObject<DirectionalLight>();
      world_light.mDirection = XYZ(Transformed(XYZ0(world_light.mDirection), world_transformation_matrix));
      scene_elements.mDirectionalLights.push_back(world_light);
    }
  }

  constexpr auto MaxPrimitivesAtLeafNodes = 1;
  constexpr auto MaxLevels = 5; // There needs to be a max level, because bboxes can be inter-contained or coincident
  scene_elements.mDrawableWorldAABBoxesOctree
      = Octree<ObjectRayTracingInSpace> { MakeSpan(scene_elements.mDrawables), MaxPrimitivesAtLeafNodes, MaxLevels };

  return scene_elements;
}

}