#pragma once

#include "ez/Camera.h"
#include "ez/Color.h"
#include "ez/DirectionalLight.h"
#include "ez/ETextHAlignment.h"
#include "ez/ETextVAlignment.h"
#include "ez/Framebuffer.h"
#include "ez/Macros.h"
#include "ez/Material3D.h"
#include "ez/Math.h"
#include "ez/MeshDrawData.h"
#include "ez/OrthographicCamera.h"
#include "ez/PerspectiveCamera.h"
#include "ez/Plane.h"
#include "ez/PointLight.h"
#include "ez/RendererGPU.h"
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
class Font;
class MeshDrawData;
class ShaderProgram;

class Renderer3D final : public RendererGPU
{
public:
  enum class EStateId
  {
    CULL_FACE_ENABLED,
    CAMERA,
    TRANSFORM_MATRIX,
    MATERIAL,
    SCENE_AMBIENT_COLOR,
    DIRECTIONAL_LIGHTS,
    POINT_LIGHTS
  };

  Renderer3D();
  Renderer3D(const Renderer3D& inRHS) = default;
  Renderer3D& operator=(const Renderer3D& inRHS) = default;
  Renderer3D(Renderer3D&&) = default;
  Renderer3D& operator=(Renderer3D&& inRHS) = default;
  ~Renderer3D() override = default;

  // Cull face enabled
  void SetCullFaceEnabled(const bool inCullFaceEnabled);
  bool GetCullFaceEnabled() const { return mState.GetCurrent<Renderer3D::EStateId::CULL_FACE_ENABLED>(); }
  void PushCullFaceEnabled() { mState.PushTop<Renderer3D::EStateId::CULL_FACE_ENABLED>(); }
  void PopCullFaceEnabled() { mState.Pop<Renderer3D::EStateId::CULL_FACE_ENABLED>(); }
  void ResetCullFaceEnabled() { mState.Reset<Renderer3D::EStateId::CULL_FACE_ENABLED>(); }

  // Camera
  void SetCamera(const std::shared_ptr<Camera3f>& inCamera);
  std::shared_ptr<Camera3f> GetCamera();
  std::shared_ptr<const Camera3f> GetCamera() const;
  std::shared_ptr<PerspectiveCameraf> GetPerspectiveCamera();                // Null if it is not a PerspectiveCamera
  std::shared_ptr<const PerspectiveCameraf> GetPerspectiveCamera() const;    // Null if it is not a PerspectiveCamera
  std::shared_ptr<OrthographicCamera3f> GetOrthographicCamera();             // Null if it is not an OrthographicCamera
  std::shared_ptr<const OrthographicCamera3f> GetOrthographicCamera() const; // Null if it is not an OrthographicCamera
  void PushCamera() { mState.PushTop<Renderer3D::EStateId::CAMERA>(); }
  void PopCamera() { mState.Pop<Renderer3D::EStateId::CAMERA>(); }
  void ResetCamera() { mState.Reset<Renderer3D::EStateId::CAMERA>(); }
  void AdaptCameraToWindow(const Window& inWindow);

  // Transformation
  void SetTransformMatrix(const Mat4f& inTransformMatrix);
  const Mat4f& GetTransformMatrix() const { return mState.GetCurrent<Renderer3D::EStateId::TRANSFORM_MATRIX>(); }
  Mat4f& GetTransformMatrix() { return mState.GetCurrent<Renderer3D::EStateId::TRANSFORM_MATRIX>(); }
  void Translate(const Vec3f& inTranslation);
  void Rotate(const Quatf& inRotation);
  void Scale(const Vec3f& inScale);
  void Scale(const float inScale);
  void PushTransformMatrix() { mState.PushTop<Renderer3D::EStateId::TRANSFORM_MATRIX>(); }
  void PopTransformMatrix() { mState.Pop<Renderer3D::EStateId::TRANSFORM_MATRIX>(); }
  void ResetTransformMatrix() { mState.Reset<Renderer3D::EStateId::TRANSFORM_MATRIX>(); }

  // Materials
  void SetMaterial(const Material3D& inMaterial);
  const Material3D& GetMaterial() const { return mState.GetCurrent<Renderer3D::EStateId::MATERIAL>(); }
  Material3D& GetMaterial() { return mState.GetCurrent<Renderer3D::EStateId::MATERIAL>(); }
  void PushMaterial() { mState.PushTop<Renderer3D::EStateId::MATERIAL>(); }
  void PopMaterial() { mState.Pop<Renderer3D::EStateId::MATERIAL>(); }
  void ResetMaterial() { mState.Reset<Renderer3D::EStateId::MATERIAL>(); }

  // Lighting
  void SetSceneAmbientColor(const Color3f& inSceneAmbientColor);
  const Color3f& GetSceneAmbientColor() const { return mState.GetCurrent<Renderer3D::EStateId::SCENE_AMBIENT_COLOR>(); }
  void PushSceneAmbientColor() { mState.PushTop<Renderer3D::EStateId::SCENE_AMBIENT_COLOR>(); }
  void PopSceneAmbientColor() { mState.Pop<Renderer3D::EStateId::SCENE_AMBIENT_COLOR>(); }
  void ResetSceneAmbientColor() { mState.Reset<Renderer3D::EStateId::SCENE_AMBIENT_COLOR>(); }

  void AddDirectionalLight(const Vec3f& inDirection, const Color3f& inColor);
  const auto& GetDirectionalLights() { return mState.GetCurrent<Renderer3D::EStateId::DIRECTIONAL_LIGHTS>(); }
  void PushDirectionalLights() { mState.PushTop<Renderer3D::EStateId::DIRECTIONAL_LIGHTS>(); }
  void PopDirectionalLights() { mState.Pop<Renderer3D::EStateId::DIRECTIONAL_LIGHTS>(); }
  void ResetDirectionalLights() { mState.Reset<Renderer3D::EStateId::DIRECTIONAL_LIGHTS>(); }

  void AddPointLight(const Vec3f& inPosition, const float inRange, const Color3f& inColor);
  const auto& GetPointLights() { return mState.GetCurrent<Renderer3D::EStateId::POINT_LIGHTS>(); }
  void PushPointLights() { mState.PushTop<Renderer3D::EStateId::POINT_LIGHTS>(); }
  void PopPointLights() { mState.Pop<Renderer3D::EStateId::POINT_LIGHTS>(); }
  void ResetPointLights() { mState.Reset<Renderer3D::EStateId::POINT_LIGHTS>(); }

  // Draw - 3D
  void AdaptToWindow(const Window& inWindow);
  void DrawMesh(const Mesh& inMesh, const RendererGPU::EDrawType inDrawType = RendererGPU::EDrawType::SOLID);
  void DrawMesh(const MeshDrawData& inMeshDrawData, const RendererGPU::EDrawType inDrawType = RendererGPU::EDrawType::SOLID);
  void DrawVAOElements(const VAO& inVAO,
      const GL::Size inNumberOfElementsToDraw,
      const GL::EPrimitivesType inPrimitivesType = GL::EPrimitivesType::TRIANGLES);
  void DrawVAOArrays(const VAO& inVAO,
      const GL::Size inNumberOfPrimitivesToDraw,
      const GL::EPrimitivesType inPrimitivesType = GL::EPrimitivesType::TRIANGLES,
      const GL::Size inBeginPrimitiveIndex = 0);
  void DrawArrow(const Segment3f& inArrowSegment);
  void DrawAxes();
  void DrawRay(const Ray3f& inRay, const float inDistance = 999999.9f);
  void DrawPlane(const Planef& inPlane, const float inPlaneSize = 5.0f);
  void DrawPoint(const Vec3f& inPoint);
  void DrawPoints(const Span<Vec3f>& inPoints);
  void DrawSegment(const Segment3f& inSegment);
  void DrawSegments(const Span<Segment3f>& inSegments);
  void DrawLineStrip(const Span<Vec3f>& inLinePoints);
  void DrawTriangle(const Triangle3f& inTriangle);
  void DrawTriangles(const Span<Triangle3f>& inTriangles);
  void DrawTriangleBoundary(const Triangle3f& inTriangle);
  void DrawAABox(const AABoxf& inAABox);
  void DrawAABoxBoundary(const AABoxf& inAABox);
  void DrawAABox();
  void DrawAABoxBoundary();
  void DrawAARect();
  void DrawAARect(const AARectf& inAARect);
  void DrawAARectBoundary();
  void DrawAARectBoundary(const AARectf& inAARect);
  void DrawCylinder(std::size_t inNumLongitudes = 32);
  void DrawTorus(std::size_t inNumLatitudes = 32, std::size_t inNumLongitudes = 32, float inHoleSize = 0.5f);
  void DrawCone(std::size_t inNumLongitudes = 32);
  void DrawHemisphere(std::size_t inNumLatitudes = 32, std::size_t inNumLongitudes = 32);
  void DrawSphere(std::size_t inNumLatitudes = 32, std::size_t inNumLongitudes = 32);
  void DrawSphere(const Spheref& inSphere, std::size_t inNumLatitudes = 32, std::size_t inNumLongitudes = 32);
  void DrawCircle(std::size_t inNumVertices = 32);
  void DrawCircleBoundary(std::size_t inNumVertices = 32);
  void DrawCircleSection(const AngleRads inAngle, std::size_t inNumVertices = 32);
  void DrawCircleSectionBoundary(const AngleRads inAngle, std::size_t inNumVertices = 32);
  void DrawText(const std::string_view inText,
      const Font& inFont,
      const float inScale = 1.0f,
      const ETextHAlignment& inHAlignment = ETextHAlignment::CENTER,
      const ETextVAlignment& inVAlignment = ETextVAlignment::CENTER,
      bool inBillboard = false,
      bool inConstantScale = false);

  // State
  using StateTupleOfStacks = TupleOfStacks<Renderer3D::EStateId,
      bool,                              // EStateId::CULL_FACE_ENABLED
      std::shared_ptr<Camera3f>,         // EStateId::CAMERA
      Mat4f,                             // EStateId::TRANSFORM_MATRIX
      Material3D,                        // EStateId::MATERIAL
      Color3f,                           // EStateId::SCENE_AMBIENT_COLOR
      std::vector<GLSLDirectionalLight>, // EStateId::DIRECTIONAL_LIGHTS
      std::vector<GLSLPointLight>>;      // EStateId::POINT_LIGHTS
  using State = RendererStateStacks<Renderer3D, StateTupleOfStacks>;
  friend class RendererStateStacks<Renderer3D, StateTupleOfStacks>;

  void PushState() final override;
  void PopState() final override;
  State& GetState() { return mState; }
  const State& GetState() const { return mState; }

protected:
  // State
  void PushAllDefaultStateValues() final override;

private:
  // Static resources
  static bool sStaticResourcesInited;
  static std::shared_ptr<ShaderProgram> sOnlyColorShaderProgram;
  static std::shared_ptr<ShaderProgram> sMeshShaderProgram;
  static std::shared_ptr<ShaderProgram> sTextShaderProgram;
  static std::shared_ptr<MeshDrawData> sCone;

  // State
  State mState { *this };

  // Lights
  static constexpr auto MaxNumberOfDirectionalLights = 100;
  static constexpr auto MaxNumberOfPointLights = 100;
  UBO mDirectionalLightsUBO;
  UBO mPointLightsUBO;

  // DrawSetup3D
  struct DrawSetup3D : public DrawSetup
  {
  private:
    Material3D::GLGuardType mMaterial3DGuard;
    GLEnableGuard<GL::EEnablable::CULL_FACE> mCullFaceEnabledGuard;
  };

  // State functions
  template <Renderer3D::EStateId StateId>
  static void ApplyState(const State::ValueType<StateId>& inValue, State& ioState);

  template <Renderer3D::EStateId StateId>
  static State::ValueType<StateId> GetDefaultValue();

  // DrawSetup3D functions
  virtual std::unique_ptr<DrawSetup> CreateDrawSetup() const override { return std::make_unique<DrawSetup3D>(); }
  virtual void PrepareForDraw(DrawSetup& ioDrawSetup) override;
};

// clang-format off
template <> struct RendererFromEStateId<Renderer3D::EStateId> { using Type = Renderer3D; };
// clang-format on
}

#include "ez/Renderer3D.tcc"