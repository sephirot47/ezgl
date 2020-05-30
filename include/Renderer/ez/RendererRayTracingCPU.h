#pragma once

#include "ez/HyperSphere.h"
#include "ez/Image2D.h"
#include "ez/ObjectRayTracing.h"
#include "ez/ObjectRayTracingInSpace.h"
#include "ez/Octree.h"
#include "ez/OrthographicCamera.h"
#include "ez/PerspectiveCamera.h"
#include "ez/Ray.h"
#include "ez/Renderer.h"
#include "ez/SceneGraphNode.h"
#include "ez/Window.h"
#include <functional>

namespace ez
{
class RendererRayTracingCPU final : public Renderer
{
public:
  enum class EStateId
  {
    CAMERA,
  };

  RendererRayTracingCPU();

  void Clear(const Color4f& inClearColor = Black<Color4f>());

  void AdaptCameraAspectRatio();
  void SetSize(const Vec2ul& inSize);
  Vec2ul GetSize() const;

  void DrawScene(const SceneGraphNode<ObjectRayTracing>& inScene);

  // Camera
  void SetCamera(const std::shared_ptr<Camera3f>& inCamera);
  std::shared_ptr<Camera3f> GetCamera();
  std::shared_ptr<const Camera3f> GetCamera() const;
  std::shared_ptr<PerspectiveCameraf> GetPerspectiveCamera();                // Null if it is not a PerspectiveCamera
  std::shared_ptr<const PerspectiveCameraf> GetPerspectiveCamera() const;    // Null if it is not a PerspectiveCamera
  std::shared_ptr<OrthographicCamera3f> GetOrthographicCamera();             // Null if it is not an OrthographicCamera
  std::shared_ptr<const OrthographicCamera3f> GetOrthographicCamera() const; // Null if it is not an OrthographicCamera
  void PushCamera() { mState.PushTop<RendererRayTracingCPU::EStateId::CAMERA>(); }
  void PopCamera() { mState.Pop<RendererRayTracingCPU::EStateId::CAMERA>(); }
  void ResetCamera() { mState.Reset<RendererRayTracingCPU::EStateId::CAMERA>(); }

  const Image2D<Color4f>& GetRenderTarget() { return mRenderTarget; }

  // State
  using StateTupleOfStacks = TupleOfStacks<RendererRayTracingCPU::EStateId,
      std::shared_ptr<Camera3f>>; // EStateId::CAMERA

  using State = RendererStateStacks<RendererRayTracingCPU, StateTupleOfStacks>;
  friend class RendererStateStacks<RendererRayTracingCPU, StateTupleOfStacks>;

  void PushState();
  void PopState();
  State& GetState() { return mState; }
  const State& GetState() const { return mState; }
  void ResetState();

protected:
  // State
  void PushAllDefaultStateValues();

private:
  Image2D<Color4f> mRenderTarget;

  // State
  State mState { *this };

  // State functions
  template <RendererRayTracingCPU::EStateId StateId>
  static void ApplyState(const State::ValueType<StateId>& inValue, State& ioState);

  template <RendererRayTracingCPU::EStateId StateId>
  static State::ValueType<StateId> GetDefaultValue();

  struct SceneElements
  {
    Octree<ObjectRayTracingInSpace> mDrawableWorldAABBoxesOctree;
    std::vector<ObjectRayTracingInSpace> mDrawables;
    std::vector<DirectionalLight> mDirectionalLights;
  };

  struct IntersectionResult
  {
    Color3f mColor = Zero<Color3f>();
    float mDistance = Max<float>();
  };

  template <EIntersectMode TIntersectMode>
  auto RayTraceInScene(const SceneElements& inSceneElements, const Ray3f& inRay, const std::size_t inCurrentBounces);

  void RayTraceFullRenderTarget(const std::function<Color4f(const Ray3f&)>& inRayTraceFunction);
  SceneElements CreateSceneElements(const SceneGraphNode<ObjectRayTracing>& inScene) const;
};
}

#include "ez/RendererRayTracingCPU.tcc"