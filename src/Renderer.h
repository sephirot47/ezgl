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
#include <type_traits>

namespace egl
{
class MeshDrawData;
class ShaderProgram;
class Camera;
class ShaderProgram;
class Texture2D;

class Renderer
{
public:
  enum class EStateId
  {
    CAMERA,
    OVERRIDE_SHADER_PROGRAM,
    RENDER_TEXTURE,
    DEPTH_ENABLED,
    BLEND_ENABLED,
    BLEND_SOURCE_FACTOR,
    BLEND_DEST_FACTOR,
    POINT_SIZE,
    LINE_WIDTH
  };

  enum class EDrawType
  {
    SOLID,
    WIREFRAME,
    POINTS
  };

protected:
  Renderer();
  Renderer(const Renderer& inRHS) = default;
  Renderer& operator=(const Renderer& inRHS) = default;
  Renderer(Renderer&&) = default;
  Renderer& operator=(Renderer&& inRHS) = default;
  virtual ~Renderer() = default;

public:
  // Clears
  void ClearBackground(const Color4f& inClearColor);
  void ClearDepth(const float inClearDepth = 1.0f);
  void Clear(const Color4f& inClearColor = Black(), const float inClearDepth = 1.0f);

  // Depth enabled
  void SetDepthTestEnabled(const bool inDepthTestEnabled);
  bool GetDepthTestEnabled() const { return mState.GetCurrent<Renderer::EStateId::DEPTH_ENABLED>(); }
  void PushDepthTestEnabled() { mState.PushTop<Renderer::EStateId::DEPTH_ENABLED>(); }
  void PopDepthTestEnabled() { mState.Pop<Renderer::EStateId::DEPTH_ENABLED>(); }
  void ResetDepthEnabled() { mState.Reset<Renderer::EStateId::DEPTH_ENABLED>(); }

  // Blend enabled
  void SetBlendEnabled(const bool inBlendEnabled);
  bool GetBlendEnabled() const { return mState.GetCurrent<Renderer::EStateId::BLEND_ENABLED>(); }
  void PushBlendEnabled() { mState.PushTop<Renderer::EStateId::BLEND_ENABLED>(); }
  void PopBlendEnabled() { mState.Pop<Renderer::EStateId::BLEND_ENABLED>(); }
  void ResetBlendEnabled() { mState.Reset<Renderer::EStateId::BLEND_ENABLED>(); }

  // Blend func/factors
  void SetBlendFunc(const GL::EBlendFactor inBlendSourceFactor, const GL::EBlendFactor inBlendDestFactor);
  GL::EBlendFactor GetBlendSourceFactor() const { return mState.GetCurrent<Renderer::EStateId::BLEND_SOURCE_FACTOR>(); }
  void PushBlendSourceFactor() { mState.PushTop<Renderer::EStateId::BLEND_SOURCE_FACTOR>(); }
  void PopBlendSourceFactor() { mState.Pop<Renderer::EStateId::BLEND_SOURCE_FACTOR>(); }
  void ResetBlendSourceactor() { mState.Reset<Renderer::EStateId::BLEND_SOURCE_FACTOR>(); }
  GL::EBlendFactor GetBlendDestFactor() const { return mState.GetCurrent<Renderer::EStateId::BLEND_DEST_FACTOR>(); }
  void PushBlendDestFactor() { mState.PushTop<Renderer::EStateId::BLEND_DEST_FACTOR>(); }
  void PopBlendDestFactor() { mState.Pop<Renderer::EStateId::BLEND_DEST_FACTOR>(); }
  void ResetBlendDestFactor() { mState.Reset<Renderer::EStateId::BLEND_DEST_FACTOR>(); }

  // Point and Line properties
  void SetPointSize(const float inPointSize);
  float GetPointSize() const { return mState.GetCurrent<Renderer::EStateId::POINT_SIZE>(); }
  void PushPointSize() { mState.PushTop<Renderer::EStateId::POINT_SIZE>(); }
  void PopPointSize() { mState.Pop<Renderer::EStateId::POINT_SIZE>(); }
  void ResetPointSize() { mState.Reset<Renderer::EStateId::POINT_SIZE>(); }

  void SetLineWidth(const float inLineWidth);
  float GetLineWidth() const { return mState.GetCurrent<Renderer::EStateId::LINE_WIDTH>(); }
  void PushLineWidth() { mState.PushTop<Renderer::EStateId::LINE_WIDTH>(); }
  void PopLineWidth() { mState.Pop<Renderer::EStateId::LINE_WIDTH>(); }
  void ResetLineWidth() { mState.Reset<Renderer::EStateId::LINE_WIDTH>(); }

  // Override ShaderProgram
  void SetOverrideShaderProgram(const std::shared_ptr<ShaderProgram>& inShaderProgram);
  std::shared_ptr<const ShaderProgram> GetOverrideShaderProgram() const
  {
    return mState.GetCurrent<Renderer::EStateId::OVERRIDE_SHADER_PROGRAM>();
  }
  std::shared_ptr<ShaderProgram> GetOverrideShaderProgram()
  {
    return mState.GetCurrent<Renderer::EStateId::OVERRIDE_SHADER_PROGRAM>();
  }
  void PushOverrideShaderProgram() { mState.PushTop<Renderer::EStateId::OVERRIDE_SHADER_PROGRAM>(); }
  void PopOverrideShaderProgram() { mState.Pop<Renderer::EStateId::OVERRIDE_SHADER_PROGRAM>(); }
  void ResetOverrideShaderProgram() { mState.Reset<Renderer::EStateId::OVERRIDE_SHADER_PROGRAM>(); }

  // RenderTexture
  void SetRenderTexture(const std::shared_ptr<Texture2D>& inRenderTexture);
  std::shared_ptr<const Texture2D> GetRenderTexture() const
  {
    return mState.GetCurrent<Renderer::EStateId::RENDER_TEXTURE>();
  }
  std::shared_ptr<Texture2D> GetRenderTexture() { return mState.GetCurrent<Renderer::EStateId::RENDER_TEXTURE>(); }
  void PushRenderTexture() { mState.PushTop<Renderer::EStateId::RENDER_TEXTURE>(); }
  void PopRenderTexture() { mState.Pop<Renderer::EStateId::RENDER_TEXTURE>(); }
  void ResetRenderTexture() { mState.Reset<Renderer::EStateId::RENDER_TEXTURE>(); }

  // Camera
  void SetCamera(const std::shared_ptr<Camera>& inCamera);
  std::shared_ptr<Camera> GetCamera();
  std::shared_ptr<const Camera> GetCamera() const;
  std::shared_ptr<PerspectiveCamera> GetPerspectiveCamera();               // Null if it is not a PerspectiveCamera
  std::shared_ptr<const PerspectiveCamera> GetPerspectiveCamera() const;   // Null if it is not a PerspectiveCamera
  std::shared_ptr<OrthographicCamera> GetOrthographicCamera();             // Null if it is not an OrthographicCamera
  std::shared_ptr<const OrthographicCamera> GetOrthographicCamera() const; // Null if it is not an OrthographicCamera
  void PushCamera() { mState.PushTop<Renderer::EStateId::CAMERA>(); }
  void PopCamera() { mState.Pop<Renderer::EStateId::CAMERA>(); }
  void ResetCamera() { mState.Reset<Renderer::EStateId::CAMERA>(); }

  // State
  using StateTupleOfStacks = TupleOfStacks<EStateId,
      std::shared_ptr<Camera>,        // EStateId::CAMERA
      std::shared_ptr<ShaderProgram>, // EStateId::OVERRIDE_SHADER_PROGRAM
      std::shared_ptr<Texture2D>,     // EStateId::RENDER_TEXTURE
      bool,                           // EStateId::DEPTH_ENABLED
      bool,                           // EStateId::BLEND_ENABLED
      GL::EBlendFactor,               // EStateId::BLEND_SOURCE_FACTOR
      GL::EBlendFactor,               // EStateId::BLEND_DEST_FACTOR
      float,                          // EStateId::POINT_SIZE
      float>;                         // EStateId::LINE_WIDTH
  using State = RendererStateStacks<Renderer, StateTupleOfStacks>;
  friend class RendererStateStacks<Renderer, StateTupleOfStacks>;

  virtual void PushState();
  virtual void PopState();
  virtual void ResetState();
  State& GetState() { return mState; }
  const State& GetState() const { return mState; }

protected:
  // Draw helpers
  virtual void Begin(const Window& inWindow);
  void DrawMesh(ShaderProgram& ioShaderProgram,
      const Mesh& inMesh,
      const Renderer::EDrawType inDrawType = Renderer::EDrawType::SOLID);
  void DrawMesh(ShaderProgram& ioShaderProgram,
      const MeshDrawData& inMeshDrawData,
      const Renderer::EDrawType inDrawType = Renderer::EDrawType::SOLID);
  void DrawVAOElements(ShaderProgram& ioShaderProgram,
      const VAO& inVAO,
      const GL::Size inNumberOfElementsToDraw,
      const GL::EPrimitivesType inPrimitivesType = GL::EPrimitivesType::TRIANGLES);
  void DrawVAOArrays(ShaderProgram& ioShaderProgram,
      const VAO& inVAO,
      const GL::Size inNumberOfPrimitivesToDraw,
      const GL::EPrimitivesType inPrimitivesType = GL::EPrimitivesType::TRIANGLES,
      const GL::Size inBeginPrimitiveIndex = 0);
  void DrawVAOArraysOrElements(ShaderProgram& ioShaderProgram,
      const VAO& inVAO,
      const GL::Size inNumberOfElementsToDraw,
      const GL::EPrimitivesType inPrimitivesType,
      const bool inDrawArrays,
      const GL::Size inBeginArraysPrimitiveIndex);
  template <typename T, std::size_t N>
  void DrawSegmentGeneric(const Segment<T, N>& inSegment, ShaderProgram& ioShaderProgram);
  template <typename T, std::size_t N>
  void DrawSegmentsGeneric(const Span<Segment<T, N>>& inSegments, ShaderProgram& ioShaderProgram);
  template <typename T, std::size_t N>
  void DrawPointGeneric(const Vec<T, N>& inPoint, ShaderProgram& ioShaderProgram);
  template <typename T, std::size_t N>
  void DrawPointsGeneric(const Span<Vec<T, N>>& inPoints, ShaderProgram& ioShaderProgram);

  // Helpers or common functionality
  using UseShaderProgramBindGuard = GLCompositeGuard<ShaderProgram, Material3D>;
  [[nodiscard]] virtual UseShaderProgramBindGuard UseShaderProgram(ShaderProgram& ioShaderProgram);

private:
  // Static resources
  static bool sStaticResourcesInited;

  // State
  State mState { *this };

  template <Renderer::EStateId StateId>
  static void ApplyState(const State::ValueType<StateId>& inValue, State& ioState);

  template <Renderer::EStateId StateId>
  static State::ValueType<StateId> GetDefaultValue();

  // Render texture framebuffer
  std::unique_ptr<Framebuffer> mRenderTextureFramebuffer;
};

// Renderer state guards
template <typename TRenderer, typename TEStateId, TEStateId StateId>
class RendererStateGuard final
{
public:
  RendererStateGuard(TRenderer& ioRenderer) : mRenderer(ioRenderer)
  {
    mRenderer.TRenderer::template GetState().template PushTop<StateId>();
  }
  RendererStateGuard(TRenderer* ioRenderer) : RendererStateGuard(*ioRenderer) {}
  ~RendererStateGuard() { mRenderer.TRenderer::template GetState().template Pop<StateId>(); }

private:
  TRenderer& mRenderer;
};

template <typename TRenderer>
class RendererStateGuardAll final
{
public:
  RendererStateGuardAll(TRenderer& ioRenderer) : mRenderer(ioRenderer) { mRenderer.TRenderer::template PushState(); }
  RendererStateGuardAll(TRenderer* ioRenderer) : RendererStateGuardAll(*ioRenderer) {}
  ~RendererStateGuardAll() { mRenderer.TRenderer::template PopState(); }

private:
  TRenderer& mRenderer;
};

// clang-format off
template <typename T> struct StateIdRenderer { static_assert(!std::is_same_v<T, T>); using Type = void; };
template <> struct StateIdRenderer<Renderer::EStateId> { using Type = Renderer; };
template <typename T> using StateIdRenderer_t = typename StateIdRenderer<T>::Type;
// clang-format on

#define RENDERER_STATE_GUARD_ALL(RENDERER_OBJECT)                                                                      \
  RendererStateGuardAll ANONYMOUS_VARIABLE_NAME() { RENDERER_OBJECT };                                                 \
  UNUSED(ANONYMOUS_VARIABLE_NAME());
#define RENDERER_STATE_GUARD(RENDERER_OBJECT, STATE_ID)                                                                \
  RendererStateGuard<StateIdRenderer_t<std::remove_pointer_t<std::decay_t<decltype(STATE_ID)>>>,                       \
      std::remove_pointer_t<std::decay_t<decltype(STATE_ID)>>,                                                         \
      STATE_ID>                                                                                                        \
  ANONYMOUS_VARIABLE_NAME() { RENDERER_OBJECT };                                                                       \
  UNUSED(ANONYMOUS_VARIABLE_NAME());
}

#include "Renderer.tcc"