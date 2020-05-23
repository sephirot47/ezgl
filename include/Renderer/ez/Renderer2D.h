#pragma once

#include "ez/Camera.h"
#include "ez/Color.h"
#include "ez/DirectionalLight.h"
#include "ez/Framebuffer.h"
#include "ez/Macros.h"
#include "ez/Material2D.h"
#include "ez/Math.h"
#include "ez/MeshDrawData.h"
#include "ez/OrthographicCamera.h"
#include "ez/PerspectiveCamera.h"
#include "ez/Plane.h"
#include "ez/PointLight.h"
#include "ez/Renderer.h"
#include "ez/RendererStateStacks.h"
#include "ez/Segment.h"
#include "ez/ShaderProgram.h"
#include "ez/Texture2D.h"
#include "ez/Triangle.h"
#include "ez/UBO.h"
#include "ez/Window.h"
#include <any>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <stack>
#include <tuple>

namespace ez
{
class MeshDrawData;
class ShaderProgram;

class Renderer2D final : public Renderer
{
public:
  enum class EStateId
  {
    CAMERA,
    TRANSFORM_MATRIX,
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
  void AdaptCameraToWindow(const Window& inWindow);

  // Transformation
  void SetTransformMatrix(const Mat3f& inTransformMatrix);
  const Mat3f& GetTransformMatrix() const { return mState.GetCurrent<Renderer2D::EStateId::TRANSFORM_MATRIX>(); }
  Mat3f& GetTransformMatrix() { return mState.GetCurrent<Renderer2D::EStateId::TRANSFORM_MATRIX>(); }
  void Translate(const Vec2f& inTranslation);
  void Rotate(const AngleRads inRotationAngle);
  void Scale(const Vec2f& inScale);
  void Scale(const float inScale);
  void PushTransformMatrix() { mState.PushTop<Renderer2D::EStateId::TRANSFORM_MATRIX>(); }
  void PopTransformMatrix() { mState.Pop<Renderer2D::EStateId::TRANSFORM_MATRIX>(); }
  void ResetTransformMatrix() { mState.Reset<Renderer2D::EStateId::TRANSFORM_MATRIX>(); }

  // Materials
  void SetMaterial(const Material2D& inMaterial);
  const Material2D& GetMaterial() const { return mState.GetCurrent<Renderer2D::EStateId::MATERIAL>(); }
  Material2D& GetMaterial() { return mState.GetCurrent<Renderer2D::EStateId::MATERIAL>(); }
  void PushMaterial() { mState.PushTop<Renderer2D::EStateId::MATERIAL>(); }
  void PopMaterial() { mState.Pop<Renderer2D::EStateId::MATERIAL>(); }
  void ResetMaterial() { mState.Reset<Renderer2D::EStateId::MATERIAL>(); }

  // Draw
  void AdaptToWindow(const Window& inWindow);
  void DrawMesh(const Mesh& inMesh, const Renderer::EDrawType inDrawType = Renderer::EDrawType::SOLID);
  void DrawMesh(const MeshDrawData& inMeshDrawData, const Renderer::EDrawType inDrawType = Renderer::EDrawType::SOLID);
  void DrawPoint(const Vec2f& inPoint);
  void DrawPoints(const Span<Vec2f>& inPoints);
  void DrawSegment(const Segment2f& inSegment);
  void DrawCircleSection(const AngleRads inAngle, const std::size_t inNumVertices = 32);
  void DrawCircleSectionBoundary(const AngleRads inAngle, const std::size_t inNumVertices = 32);
  void DrawCircle(const std::size_t inNumVertices = 32);
  void DrawCircleBoundary(const std::size_t inNumVertices = 32);
  void DrawSegments(const Span<Segment2f>& inSegments);
  void DrawLineStrip(const Span<Vec2f>& inLinePoints);
  void DrawTriangle(const Triangle2f& inTriangle);
  void DrawTriangles(const Span<Triangle2f>& inTriangles);
  void DrawTriangleBoundary(const Triangle2f& inTriangle);
  void DrawAARect();
  void DrawAARect(const AARectf& inAARect);
  void DrawAARectBoundary();
  void DrawAARectBoundary(const AARectf& inAARect);

  // State
  using StateTupleOfStacks = TupleOfStacks<Renderer2D::EStateId,
      std::shared_ptr<OrthographicCamera2f>, // EStateId::CAMERA
      Mat3f,                                 // EStateId::TRANSFORM_MATRIX
      Material2D>;                           // EStateId::MATERIAL
  using State = RendererStateStacks<Renderer2D, StateTupleOfStacks>;
  friend class RendererStateStacks<Renderer2D, StateTupleOfStacks>;

  void PushState() final override;
  void PopState() final override;
  State& GetState() { return mState; }
  const State& GetState() const { return mState; }

private:
  // Static resources
  static bool sStaticResourcesInited;
  static std::shared_ptr<ShaderProgram> sShaderProgram;

  // State
  State mState { *this };

  // DrawSetup2D
  class DrawSetup2D : public DrawSetup
  {
  private:
    Material2D::GLGuardType mMaterial2DGuard;
    GLEnableGuard<GL::EEnablable::CULL_FACE> mCullFaceEnableGuard;
  };

  // State functions
  template <Renderer2D::EStateId StateId>
  static void ApplyState(const State::ValueType<StateId>& inValue, State& ioState);

  template <Renderer2D::EStateId StateId>
  static State::ValueType<StateId> GetDefaultValue();

  void PushAllDefaultStateValues() final override;

  // Draw setup functions
  virtual std::unique_ptr<DrawSetup> CreateDrawSetup() const override { return std::make_unique<DrawSetup2D>(); }
  virtual void PrepareForDraw(DrawSetup& ioDrawSetupPointer) override;
};

// clang-format off
template <> struct RendererFromEStateId<Renderer2D::EStateId> { using Type = Renderer2D; };
// clang-format on
}

#include "ez/Renderer2D.tcc"