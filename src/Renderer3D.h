#pragma once

#include "Camera.h"
#include "Color.h"
#include "DirectionalLight.h"
#include "Framebuffer.h"
#include "Macros.h"
#include "Material3D.h"
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

class Renderer3D final : public Renderer
{
public:
  enum class EStateId
  {
    CULL_FACE_ENABLED,
    CAMERA,
    MODEL_MATRIX,
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

  // Transformation
  void SetModelMatrix(const Mat4f& inModelMatrix);
  const Mat4f& GetModelMatrix() const { return mState.GetCurrent<Renderer3D::EStateId::MODEL_MATRIX>(); }
  Mat4f& GetModelMatrix() { return mState.GetCurrent<Renderer3D::EStateId::MODEL_MATRIX>(); }
  void Translate(const Vec3f& inTranslation);
  void Rotate(const Quatf& inRotation);
  void Scale(const Vec3f& inScale);
  void Scale(const float inScale);
  void PushModelMatrix() { mState.PushTop<Renderer3D::EStateId::MODEL_MATRIX>(); }
  void PopModelMatrix() { mState.Pop<Renderer3D::EStateId::MODEL_MATRIX>(); }
  void ResetModelMatrix() { mState.Reset<Renderer3D::EStateId::MODEL_MATRIX>(); }

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
  void Begin(const Window& inWindow);
  void DrawMesh(const Mesh& inMesh, const Renderer::EDrawType inDrawType = Renderer::EDrawType::SOLID);
  void DrawMesh(const MeshDrawData& inMeshDrawData, const Renderer::EDrawType inDrawType = Renderer::EDrawType::SOLID);
  void DrawArrow(const Segment3f& inArrowSegment);
  void DrawAxes();
  void DrawPoint(const Vec3f& inPoint);
  void DrawPoints(const Span<Vec3f>& inPoints);
  void DrawSegment(const Segment3f& inSegment);
  void DrawSegments(const Span<Segment3f>& inSegments);
  void DrawTriangle(const Triangle3f& inTriangle);

  // State
  using StateTupleOfStacks = TupleOfStacks<Renderer3D::EStateId,
      bool,                              // EStateId::CULL_FACE_ENABLED
      std::shared_ptr<Camera3f>,         // EStateId::CAMERA
      Mat4f,                             // EStateId::MODEL_MATRIX
      Material3D,                        // EStateId::MATERIAL
      Color3f,                           // EStateId::SCENE_AMBIENT_COLOR
      std::vector<GLSLDirectionalLight>, // EStateId::DIRECTIONAL_LIGHTS
      std::vector<GLSLPointLight>>;      // EStateId::POINT_LIGHTS
  using State = RendererStateStacks<Renderer3D, StateTupleOfStacks>;
  friend class RendererStateStacks<Renderer3D, StateTupleOfStacks>;

  void PushState() override;
  void PopState() override;
  void ResetState() override;
  State& GetState() { return mState; }
  const State& GetState() const { return mState; }

private:
  // Static resources
  static bool sStaticResourcesInited;
  static std::shared_ptr<ShaderProgram> sOnlyColorShaderProgram;
  static std::shared_ptr<ShaderProgram> sMeshShaderProgram;
  static std::shared_ptr<MeshDrawData> sCone;

  State mState { *this };

  template <Renderer3D::EStateId StateId>
  static void ApplyState(const State::ValueType<StateId>& inValue, State& ioState);

  template <Renderer3D::EStateId StateId>
  static State::ValueType<StateId> GetDefaultValue();

  // Lights
  static constexpr auto MaxNumberOfDirectionalLights = 100;
  static constexpr auto MaxNumberOfPointLights = 100;
  UBO mDirectionalLightsUBO;
  UBO mPointLightsUBO;

  struct DrawSetup3D : public DrawSetup
  {
  private:
    GLMultiGuard<Material3D> mGuard;
  };
  virtual std::unique_ptr<DrawSetup> CreateDrawSetup() const override { return std::make_unique<DrawSetup3D>(); }
  virtual void PrepareForDraw(DrawSetup& ioDrawSetup) override;
};

// clang-format off
template <> struct StateIdRenderer<Renderer3D::EStateId> { using Type = Renderer3D; };
// clang-format on
}

#include "Renderer3D.tcc"