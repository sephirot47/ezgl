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
    MODEL_MATRIX,
    MATERIAL,
    CULL_FACE_ENABLED,
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
  void DrawPoint(const Vec3f& inPoint) { DrawPointGeneric(inPoint, *sOnlyColorShaderProgram); }
  void DrawPoints(const Span<Vec3f>& inPoints) { DrawPointsGeneric(inPoints, *sOnlyColorShaderProgram); }
  void DrawSegment(const Segment3f& inSegment) { DrawSegmentGeneric(inSegment, *sOnlyColorShaderProgram); }
  void DrawSegments(const Span<Segment3f>& inSegments) { DrawSegmentsGeneric(inSegments, *sOnlyColorShaderProgram); }
  void DrawTriangle(const Triangle3f& inTriangle);

  // State
  using StateTupleOfStacks = TupleOfStacks<Renderer3D::EStateId,
      Mat4f,                             // ERendererStateId::MODEL_MATRIX
      Material3D,                        // ERendererStateId::MATERIAL
      bool,                              // ERendererStateId::CULL_FACE_ENABLED
      Color3f,                           // ERendererStateId::SCENE_AMBIENT_COLOR
      std::vector<GLSLDirectionalLight>, // ERendererStateId::DIRECTIONAL_LIGHTS
      std::vector<GLSLPointLight>>;      // ERendererStateId::POINT_LIGHTS
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
  static std::unique_ptr<ShaderProgram> sOnlyColorShaderProgram;
  static std::unique_ptr<ShaderProgram> sMeshShaderProgram;
  static std::unique_ptr<MeshDrawData> sCone;

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

  // Helpers or common functionality
  [[nodiscard]] virtual UseShaderProgramBindGuard UseShaderProgram(ShaderProgram& ioShaderProgram) override;
};

// clang-format off
template <> struct StateIdRenderer<Renderer3D::EStateId> { using Type = Renderer3D; };
// clang-format on
}

#include "Renderer3D.tcc"