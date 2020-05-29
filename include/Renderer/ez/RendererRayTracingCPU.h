#pragma once

#include "ez/DrawableRayTracing.h"
#include "ez/DrawableRayTracingWrapper.h"
#include "ez/HyperSphere.h"
#include "ez/Image2D.h"
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

  void DrawScene(const SceneGraphNode<DrawableRayTracing>& inScene);

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

  std::optional<Color4f> RayTraceInScene(const Octree<DrawableRayTracingWrapper>& inSceneOctree,
      const std::vector<DrawableRayTracingWrapper>& inSceneDrawables,
      const Ray3f& inRay);

  void RayTraceFullRenderTarget(const std::function<Color4f(const Ray3f&)>& inRayTraceFunction);

  void CreateSceneOctree(const SceneGraphNode<DrawableRayTracing>& inScene,
      Octree<DrawableRayTracingWrapper>& outOctree,
      std::vector<DrawableRayTracingWrapper>& outDrawables) const;
};
}

#include "ez/RendererRayTracingCPU.tcc"