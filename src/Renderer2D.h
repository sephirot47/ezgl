#pragma once

#include "Camera.h"
#include "Color.h"
#include "DirectionalLight.h"
#include "Framebuffer.h"
#include "Macros.h"
#include "Material2D.h"
#include "Math.h"
#include "MeshDrawData.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "Plane.h"
#include "PointLight.h"
#include "Renderer.h"
#include "RendererStateStacks.h"
#include "Segment.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Triangle.h"
#include "UBO.h"
#include "Window.h"
#include <any>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <stack>
#include <tuple>

namespace egl
{
class MeshDrawData;
class ShaderProgram;

class Renderer2D final : public Renderer
{
public:
  enum class EStateId
  {
    CAMERA,
    MODEL_MATRIX,
    MATERIAL,
  };

  Renderer2D();
  Renderer2D(const Renderer2D& inRHS) = default;
  Renderer2D& operator=(const Renderer2D& inRHS) = default;
  Renderer2D(Renderer2D&&) = default;
  Renderer2D& operator=(Renderer2D&& inRHS) = default;
  ~Renderer2D() override = default;

  // Camera
  void SetCamera(const std::shared_ptr<OrthographicCamera2f>& inCamera);
  std::shared_ptr<OrthographicCamera2f> GetCamera();
  std::shared_ptr<const OrthographicCamera2f> GetCamera() const;
  void PushCamera() { mState.PushTop<Renderer2D::EStateId::CAMERA>(); }
  void PopCamera() { mState.Pop<Renderer2D::EStateId::CAMERA>(); }
  void ResetCamera() { mState.Reset<Renderer2D::EStateId::CAMERA>(); }
  void AdaptCameraToWindow(const Window &inWindow);

  // Transformation
  void SetModelMatrix(const Mat3f& inModelMatrix);
  const Mat3f& GetModelMatrix() const { return mState.GetCurrent<Renderer2D::EStateId::MODEL_MATRIX>(); }
  Mat3f& GetModelMatrix() { return mState.GetCurrent<Renderer2D::EStateId::MODEL_MATRIX>(); }
  void Translate(const Vec2f& inTranslation);
  void Rotate(const AngleRads inRotationAngle);
  void Scale(const Vec2f& inScale);
  void Scale(const float inScale);
  void PushModelMatrix() { mState.PushTop<Renderer2D::EStateId::MODEL_MATRIX>(); }
  void PopModelMatrix() { mState.Pop<Renderer2D::EStateId::MODEL_MATRIX>(); }
  void ResetModelMatrix() { mState.Reset<Renderer2D::EStateId::MODEL_MATRIX>(); }

  // Materials
  void SetMaterial(const Material2D& inMaterial);
  const Material2D& GetMaterial() const { return mState.GetCurrent<Renderer2D::EStateId::MATERIAL>(); }
  Material2D& GetMaterial() { return mState.GetCurrent<Renderer2D::EStateId::MATERIAL>(); }
  void PushMaterial() { mState.PushTop<Renderer2D::EStateId::MATERIAL>(); }
  void PopMaterial() { mState.Pop<Renderer2D::EStateId::MATERIAL>(); }
  void ResetMaterial() { mState.Reset<Renderer2D::EStateId::MATERIAL>(); }

  // Draw
  void Begin(const Window& inWindow);
  void DrawMesh(const Mesh& inMesh, const Renderer::EDrawType inDrawType = Renderer::EDrawType::SOLID);
  void DrawMesh(const MeshDrawData& inMeshDrawData, const Renderer::EDrawType inDrawType = Renderer::EDrawType::SOLID);
  void DrawPoint(const Vec2f& inPoint);
  void DrawPoints(const Span<Vec2f>& inPoints);
  void DrawSegment(const Segment2f& inSegment);
  void DrawSegments(const Span<Segment2f>& inSegments);
  void DrawTriangle(const Triangle2f& inTriangle);
  void DrawTriangleBoundary(const Triangle2f& inTriangle);

  // State
  using StateTupleOfStacks = TupleOfStacks<Renderer2D::EStateId,
      std::shared_ptr<OrthographicCamera2f>, // EStateId::CAMERA
      Mat3f,                                 // EStateId::MODEL_MATRIX
      Material2D>;                           // EStateId::MATERIAL
  using State = RendererStateStacks<Renderer2D, StateTupleOfStacks>;
  friend class RendererStateStacks<Renderer2D, StateTupleOfStacks>;

  void PushState() override;
  void PopState() override;
  void ResetState() override;
  State& GetState() { return mState; }
  const State& GetState() const { return mState; }

private:
  // Static resources
  static bool sStaticResourcesInited;
  static std::shared_ptr<ShaderProgram> sShaderProgram;

  State mState { *this };

  template <Renderer2D::EStateId StateId>
  static void ApplyState(const State::ValueType<StateId>& inValue, State& ioState);

  template <Renderer2D::EStateId StateId>
  static State::ValueType<StateId> GetDefaultValue();

  class DrawSetup2D : public DrawSetup
  {
  private:
    GLMultiGuard<Material2D> mGuard;
  };
  virtual std::unique_ptr<DrawSetup> CreateDrawSetup() const override { return std::make_unique<DrawSetup2D>(); }
  virtual void PrepareForDraw(DrawSetup& ioDrawSetupPointer) override;
};

// clang-format off
template <> struct StateIdRenderer<Renderer2D::EStateId> { using Type = Renderer2D; };
// clang-format on
}

#include "Renderer2D.tcc"