#pragma once

#include "Camera.h"
#include "Color.h"
#include "DirectionalLight.h"
#include "DrawableMesh.h"
#include "Framebuffer.h"
#include "Material.h"
#include "Math.h"
#include "PointLight.h"
#include "Segment.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "TupleOfStacks.h"
#include "UBO.h"
#include <any>
#include <cstdint>
#include <memory>
#include <optional>
#include <stack>
#include <tuple>

namespace egl
{
class DrawableMesh;
class ShaderProgram;

// Renderer state id enum
enum class ERendererStateId
{
  CAMERA,
  MODEL_MATRIX,
  MATERIAL,
  OVERRIDE_SHADER_PROGRAM,
  RENDER_TEXTURE,
  DEPTH_ENABLED,
  CULL_FACE_ENABLED,
  BLEND_ENABLED,
  BLEND_SOURCE_FACTOR,
  BLEND_DEST_FACTOR,
  POINT_SIZE,
  LINE_WIDTH,
  SCENE_AMBIENT_COLOR,
  DIRECTIONAL_LIGHTS,
  POINT_LIGHTS
};

// clang-format off
template <ERendererStateId TStateId> struct RendererStateStackValueType { using Type = void; };
template <ERendererStateId TStateId>
using RendererStateStackValueType_t = typename RendererStateStackValueType<TStateId>::Type;
template <> struct RendererStateStackValueType<ERendererStateId::CAMERA> { using Type = std::shared_ptr<Camera>; };
template <> struct RendererStateStackValueType<ERendererStateId::MODEL_MATRIX> { using Type = Mat4f; };
template <> struct RendererStateStackValueType<ERendererStateId::MATERIAL> { using Type = Material; };
template <> struct RendererStateStackValueType<ERendererStateId::OVERRIDE_SHADER_PROGRAM> { using Type = std::shared_ptr<ShaderProgram>; };
template <> struct RendererStateStackValueType<ERendererStateId::RENDER_TEXTURE> { using Type = std::shared_ptr<Texture2D>; };
template <> struct RendererStateStackValueType<ERendererStateId::DEPTH_ENABLED> { using Type = bool; };
template <> struct RendererStateStackValueType<ERendererStateId::CULL_FACE_ENABLED> { using Type = bool; };
template <> struct RendererStateStackValueType<ERendererStateId::BLEND_ENABLED> { using Type = bool; };
template <> struct RendererStateStackValueType<ERendererStateId::BLEND_SOURCE_FACTOR> { using Type = GL::EBlendFactor; };
template <> struct RendererStateStackValueType<ERendererStateId::BLEND_DEST_FACTOR> { using Type = GL::EBlendFactor; };
template <> struct RendererStateStackValueType<ERendererStateId::POINT_SIZE> { using Type = float; };
template <> struct RendererStateStackValueType<ERendererStateId::LINE_WIDTH> { using Type = float; };
template <> struct RendererStateStackValueType<ERendererStateId::SCENE_AMBIENT_COLOR> { using Type = Color3f; };
template <> struct RendererStateStackValueType<ERendererStateId::DIRECTIONAL_LIGHTS> { using Type = std::vector<GLSLDirectionalLight>; };
template <> struct RendererStateStackValueType<ERendererStateId::POINT_LIGHTS> { using Type = std::vector<GLSLPointLight>; };
// clang-format on

class Renderer
{
public:
  enum class EDrawType
  {
    SOLID,
    WIREFRAME,
    POINTS
  };

  Renderer();
  Renderer(const Renderer& inRHS) = default;
  Renderer& operator=(const Renderer& inRHS) = default;
  Renderer(Renderer&&) = default;
  Renderer& operator=(Renderer&& inRHS) = default;
  ~Renderer() = default;

  // Clears
  void ClearBackground(const Color4f& inClearColor);
  void ClearDepth();

  // Depth, Cull and Blend
  void SetDepthTestEnabled(const bool inDepthTestEnabled);
  void SetCullFaceEnabled(const bool inCullFaceEnabled);
  void SetBlendEnabled(const bool inBlendEnabled);
  void SetBlendFunc(const GL::EBlendFactor inBlendSourceFactor, const GL::EBlendFactor inBlendDestFactor);

  // Point and Line properties
  void SetPointSize(const float inPointSize);
  void SetLineWidth(const float inLineWidth);

  // Override ShaderProgram
  void SetOverrideShaderProgram(const std::shared_ptr<ShaderProgram>& inShaderProgram);

  // RenderTexture
  void SetRenderTexture(const std::shared_ptr<Texture2D>& inRenderTexture);

  // Camera
  void SetCamera(const std::shared_ptr<Camera>& inCamera);
  std::shared_ptr<const Camera> GetCamera() const;
  std::shared_ptr<Camera> GetCamera();

  // Transformation
  void SetModelMatrix(const Mat4f& inModelMatrix);
  void Translate(const Vec3f& inTranslation);
  void Rotate(const Quatf& inRotation);
  void Scale(const Vec3f& inScale);
  void Scale(const float inScale);

  // Materials
  void SetMaterial(const Material& inMaterial);
  const Material& GetMaterial() const;
  Material& GetMaterial();

  // Lighting
  void SetSceneAmbientColor(const Color3f& inSceneAmbientColor);
  void AddDirectionalLight(const Vec3f& inDirection, const Color3f& inColor);
  void AddPointLight(const Vec3f& inPosition, const float inRange, const Color3f& inColor);
  void ClearDirectionalLights();
  void ClearPointLights();

  // All state
  void PushState();
  void PopState();
  void ResetState();

  // Draw - 3D
  void DrawMesh(const DrawableMesh& inDrawableMesh, const Renderer::EDrawType inDrawType = Renderer::EDrawType::SOLID);
  void DrawVAOElements(const VAO& inVAO,
      const GL::Size inNumberOfElementsToDraw,
      const GL::EPrimitivesType inPrimitivesType = GL::EPrimitivesType::TRIANGLES);
  void DrawVAOArrays(const VAO& inVAO,
      const GL::Size inNumberOfPrimitivesToDraw,
      const GL::EPrimitivesType inPrimitivesType = GL::EPrimitivesType::TRIANGLES,
      const GL::Size inBeginPrimitiveIndex = 0);
  void DrawArrow(const Segment3f& inArrowSegment);
  void DrawAxes();
  void DrawPoint(const Vec3f& inPoint);
  void DrawPoints(const Span<Vec3f>& inPoints);
  void DrawSegment(const Segment3f& inSegment);
  void DrawSegments(const Span<Segment3f>& inSegments);

  // Draw - 2D (z = 0.0)
  void DrawPoint(const Vec2f& inPoint);
  void DrawPoints(const Span<Vec2f>& inPoints);
  void DrawSegment(const Segment2f& inSegment);
  void DrawSegments(const Span<Segment2f>& inSegments);

private:
  // Static resources
  static bool sStaticResourcesInited;
  static std::unique_ptr<ShaderProgram> sOnlyColorShaderProgram;
  static std::unique_ptr<ShaderProgram> sMeshShaderProgram;
  static std::unique_ptr<DrawableMesh> sCone;
  static std::shared_ptr<Texture2D> sWhiteTexture;

  // State ===================

  template <ERendererStateId TStateId>
  std::stack<RendererStateStackValueType_t<TStateId>>& GetStateStack()
  {
    auto& state_stack = mStateStacks.GetStack<TStateId>();
    return state_stack;
  }

  template <ERendererStateId TStateId>
  const std::stack<RendererStateStackValueType_t<TStateId>>& GetStateStack() const
  {
    return const_cast<Renderer&>(*this).GetStateStack<TStateId>();
  }

  template <ERendererStateId TStateId>
  auto& GetCurrentState()
  {
    auto& state_stack = GetStateStack<TStateId>();
    EXPECTS(state_stack.size() >= 1);
    return state_stack.top();
  }

  template <ERendererStateId TStateId>
  const auto& GetCurrentState() const
  {
    return const_cast<Renderer&>(*this).GetCurrentState<TStateId>();
  }

  template <ERendererStateId TStateId>
  void ApplyState(const RendererStateStackValueType_t<TStateId>& inValue);

  // State stack values
  template <ERendererStateId TStateId>
  static RendererStateStackValueType_t<TStateId> GetDefaultStateValue();

  using StateStacksTupleType = TupleOfStacks<ERendererStateId,
      RendererStateStackValueType_t<ERendererStateId::CAMERA>,
      RendererStateStackValueType_t<ERendererStateId::MODEL_MATRIX>,
      RendererStateStackValueType_t<ERendererStateId::MATERIAL>,
      RendererStateStackValueType_t<ERendererStateId::OVERRIDE_SHADER_PROGRAM>,
      RendererStateStackValueType_t<ERendererStateId::RENDER_TEXTURE>,
      RendererStateStackValueType_t<ERendererStateId::DEPTH_ENABLED>,
      RendererStateStackValueType_t<ERendererStateId::CULL_FACE_ENABLED>,
      RendererStateStackValueType_t<ERendererStateId::BLEND_ENABLED>,
      RendererStateStackValueType_t<ERendererStateId::BLEND_SOURCE_FACTOR>,
      RendererStateStackValueType_t<ERendererStateId::BLEND_DEST_FACTOR>,
      RendererStateStackValueType_t<ERendererStateId::POINT_SIZE>,
      RendererStateStackValueType_t<ERendererStateId::LINE_WIDTH>,
      RendererStateStackValueType_t<ERendererStateId::SCENE_AMBIENT_COLOR>,
      RendererStateStackValueType_t<ERendererStateId::DIRECTIONAL_LIGHTS>,
      RendererStateStackValueType_t<ERendererStateId::POINT_LIGHTS>>;
  StateStacksTupleType mStateStacks;

  template <ERendererStateId TStateId>
  struct PushStateFunctor
  {
    void operator()(StateStacksTupleType::StackType<TStateId>& ioStack) const { ioStack.push(ioStack.top()); }
  };

  template <typename TTuple, ERendererStateId TStateId>
  void PushDefaultValueToAllStateStacks(TTuple& inTuple)
  {
    if constexpr (static_cast<int>(TStateId) < std::tuple_size<TTuple>())
    {
      auto& state_stack = std::get<static_cast<int>(TStateId)>(inTuple);
      state_stack.push(GetDefaultStateValue<TStateId>());
      PushDefaultValueToAllStateStacks<TTuple, static_cast<ERendererStateId>(static_cast<int>(TStateId) + 1)>(inTuple);
    }
  }

  template <typename TTuple, ERendererStateId TStateId>
  void ApplyAllStateStacks(const TTuple& inTuple)
  {
    if constexpr (static_cast<int>(TStateId) < std::tuple_size<TTuple>())
    {
      const auto& state_stack_current_top_element = GetCurrentState<TStateId>();
      ApplyState<TStateId>(state_stack_current_top_element);
      ApplyAllStateStacks<TTuple, static_cast<ERendererStateId>(static_cast<int>(TStateId) + 1)>(inTuple);
    }
  }

  template <ERendererStateId TStateId>
  void ResetCurrentState()
  {
    GetCurrentState<TStateId>() = GetDefaultStateValue<TStateId>();
  }

  // ==========================

  // Lights
  static constexpr auto MaxNumberOfDirectionalLights = 100;
  static constexpr auto MaxNumberOfPointLights = 100;
  UBO mDirectionalLightsUBO;
  UBO mPointLightsUBO;

  // Render texture framebuffer
  std::unique_ptr<Framebuffer> mRenderTextureFramebuffer;

  // Draw helpers
  void DrawVAOArraysOrElements(const VAO& inVAO,
      const GL::Size inNumberOfElementsToDraw,
      const GL::EPrimitivesType inPrimitivesType,
      const bool inDrawArrays,
      const GL::Size inBeginArraysPrimitiveIndex);

  // Draw - Generic (3D and 2D)
  template <typename T, std::size_t N>
  void DrawSegmentGeneric(const Segment<T, N>& inSegment);
  template <typename T, std::size_t N>
  void DrawSegmentsGeneric(const Span<Segment<T, N>>& inSegments);
  template <typename T, std::size_t N>
  void DrawPointGeneric(const Vec<T, N>& inPoint);
  template <typename T, std::size_t N>
  void DrawPointsGeneric(const Span<Vec<T, N>>& inPoints);

  // Helpers
  using UseShaderProgramBindGuard = GLCompositeGuard<ShaderProgram, Material>;
  [[nodiscard]] UseShaderProgramBindGuard UseShaderProgram(ShaderProgram& ioShaderProgram);
};
}

#include "Renderer.tcc"