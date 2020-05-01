#pragma once

#include "ezgl/Camera.h"
#include "ezgl/Color.h"
#include "ezgl/DirectionalLight.h"
#include "ezgl/Framebuffer.h"
#include "ezgl/Macros.h"
#include "ezgl/Material3D.h"
#include "ezgl/Math.h"
#include "ezgl/MeshDrawData.h"
#include "ezgl/OrthographicCamera.h"
#include "ezgl/PerspectiveCamera.h"
#include "ezgl/Plane.h"
#include "ezgl/PointLight.h"
#include "ezgl/Rect.h"
#include "ezgl/RenderTarget.h"
#include "ezgl/RendererStateStacks.h"
#include "ezgl/Segment.h"
#include "ezgl/ShaderProgram.h"
#include "ezgl/Texture2D.h"
#include "ezgl/Triangle.h"
#include "ezgl/UBO.h"
#include "ezgl/Window.h"
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
class ShaderProgram;
class Texture2D;

class Renderer
{
public:
  enum class EStateId
  {
    OVERRIDE_SHADER_PROGRAM,
    OVERRIDE_RENDER_TARGET,
    OVERRIDE_FRAMEBUFFER,
    VIEWPORT,
    DEPTH_FUNC,
    DEPTH_WRITE_ENABLED,
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
  void Clear(const Color4f& inClearColor = Black<Color4f>(), const float inClearDepth = 1.0f);

  // Depth func
  void SetDepthFunc(const GL::EDepthFunc inDepthFunc);
  GL::EDepthFunc GetDepthFunc() const { return mState.GetCurrent<EStateId::DEPTH_FUNC>(); }
  void PushDepthFunc() { mState.PushTop<EStateId::DEPTH_FUNC>(); }
  void PopDepthFunc() { mState.Pop<EStateId::DEPTH_FUNC>(); }
  void ResetDepthFunc() { mState.Reset<EStateId::DEPTH_FUNC>(); }

  // Depth write
  void SetDepthWriteEnabled(const bool inDepthWriteEnabled);
  bool GetDepthWriteEnabled() const { return mState.GetCurrent<EStateId::DEPTH_WRITE_ENABLED>(); }
  void PushDepthWriteEnabled() { mState.PushTop<EStateId::DEPTH_WRITE_ENABLED>(); }
  void PopDepthWriteEnabled() { mState.Pop<EStateId::DEPTH_WRITE_ENABLED>(); }
  void ResetDepthWriteEnabled() { mState.Reset<EStateId::DEPTH_WRITE_ENABLED>(); }

  // Blend enabled
  void SetBlendEnabled(const bool inBlendEnabled);
  bool GetBlendEnabled() const { return mState.GetCurrent<EStateId::BLEND_ENABLED>(); }
  void PushBlendEnabled() { mState.PushTop<EStateId::BLEND_ENABLED>(); }
  void PopBlendEnabled() { mState.Pop<EStateId::BLEND_ENABLED>(); }
  void ResetBlendEnabled() { mState.Reset<EStateId::BLEND_ENABLED>(); }

  // Blend func/factors
  void SetBlendFunc(const GL::EBlendFactor inBlendSourceFactor, const GL::EBlendFactor inBlendDestFactor);
  GL::EBlendFactor GetBlendSourceFactor() const { return mState.GetCurrent<EStateId::BLEND_SOURCE_FACTOR>(); }
  void PushBlendSourceFactor() { mState.PushTop<EStateId::BLEND_SOURCE_FACTOR>(); }
  void PopBlendSourceFactor() { mState.Pop<EStateId::BLEND_SOURCE_FACTOR>(); }
  void ResetBlendSourceactor() { mState.Reset<EStateId::BLEND_SOURCE_FACTOR>(); }
  GL::EBlendFactor GetBlendDestFactor() const { return mState.GetCurrent<EStateId::BLEND_DEST_FACTOR>(); }
  void PushBlendDestFactor() { mState.PushTop<EStateId::BLEND_DEST_FACTOR>(); }
  void PopBlendDestFactor() { mState.Pop<EStateId::BLEND_DEST_FACTOR>(); }
  void ResetBlendDestFactor() { mState.Reset<EStateId::BLEND_DEST_FACTOR>(); }

  // Point size
  void SetPointSize(const float inPointSize);
  float GetPointSize() const { return mState.GetCurrent<EStateId::POINT_SIZE>(); }
  void PushPointSize() { mState.PushTop<EStateId::POINT_SIZE>(); }
  void PopPointSize() { mState.Pop<EStateId::POINT_SIZE>(); }
  void ResetPointSize() { mState.Reset<EStateId::POINT_SIZE>(); }

  // Line width
  void SetLineWidth(const float inLineWidth);
  float GetLineWidth() const { return mState.GetCurrent<EStateId::LINE_WIDTH>(); }
  void PushLineWidth() { mState.PushTop<EStateId::LINE_WIDTH>(); }
  void PopLineWidth() { mState.Pop<EStateId::LINE_WIDTH>(); }
  void ResetLineWidth() { mState.Reset<EStateId::LINE_WIDTH>(); }

  // Override ShaderProgram
  void SetOverrideShaderProgram(const std::shared_ptr<ShaderProgram>& inShaderProgram);
  std::shared_ptr<ShaderProgram> GetOverrideShaderProgram()
  {
    return mState.GetCurrent<EStateId::OVERRIDE_SHADER_PROGRAM>();
  }
  std::shared_ptr<const ShaderProgram> GetOverrideShaderProgram() const
  {
    return mState.GetCurrent<EStateId::OVERRIDE_SHADER_PROGRAM>();
  }
  void PushOverrideShaderProgram() { mState.PushTop<EStateId::OVERRIDE_SHADER_PROGRAM>(); }
  void PopOverrideShaderProgram() { mState.Pop<EStateId::OVERRIDE_SHADER_PROGRAM>(); }
  void ResetOverrideShaderProgram() { mState.Reset<EStateId::OVERRIDE_SHADER_PROGRAM>(); }

  // Render Texture
  std::shared_ptr<RenderTarget> GetDefaultRenderTarget() { return mDefaultRenderTarget; }
  std::shared_ptr<const RenderTarget> GetDefaultRenderTarget() const { return mDefaultRenderTarget; }
  std::shared_ptr<RenderTarget> GetRenderTarget()
  {
    return GetOverrideRenderTarget() ? GetOverrideRenderTarget() : GetDefaultRenderTarget();
  }
  std::shared_ptr<const RenderTarget> GetRenderTarget() const
  {
    return GetOverrideRenderTarget() ? GetOverrideRenderTarget() : GetDefaultRenderTarget();
  }
  void SetOverrideRenderTarget(const std::shared_ptr<RenderTarget>& inOverrideRenderTarget);
  std::shared_ptr<RenderTarget> GetOverrideRenderTarget()
  {
    return mState.GetCurrent<EStateId::OVERRIDE_RENDER_TARGET>();
  }
  std::shared_ptr<const RenderTarget> GetOverrideRenderTarget() const
  {
    return mState.GetCurrent<EStateId::OVERRIDE_RENDER_TARGET>();
  }
  void PushOverrideRenderTarget() { mState.PushTop<EStateId::OVERRIDE_RENDER_TARGET>(); }
  void PopOverrideRenderTarget() { mState.Pop<EStateId::OVERRIDE_RENDER_TARGET>(); }
  void ResetOverrideRenderTarget() { mState.Reset<EStateId::OVERRIDE_RENDER_TARGET>(); }

  // Framebuffer
  std::shared_ptr<Framebuffer> GetDefaultFramebuffer() { return mDefaultFramebuffer; }
  std::shared_ptr<const Framebuffer> GetDefaultFramebuffer() const { return mDefaultFramebuffer; }
  std::shared_ptr<Framebuffer> GetFramebuffer()
  {
    return GetOverrideFramebuffer() ? GetOverrideFramebuffer() : GetDefaultFramebuffer();
  }
  std::shared_ptr<const Framebuffer> GetFramebuffer() const
  {
    return GetOverrideFramebuffer() ? GetOverrideFramebuffer() : GetDefaultFramebuffer();
  }
  void SetOverrideFramebuffer(const std::shared_ptr<Framebuffer>& inOverrideFramebuffer);
  std::shared_ptr<Framebuffer> GetOverrideFramebuffer() { return mState.GetCurrent<EStateId::OVERRIDE_FRAMEBUFFER>(); }
  std::shared_ptr<const Framebuffer> GetOverrideFramebuffer() const
  {
    return mState.GetCurrent<EStateId::OVERRIDE_FRAMEBUFFER>();
  }
  void PushOverrideFramebuffer() { mState.PushTop<EStateId::OVERRIDE_FRAMEBUFFER>(); }
  void PopOverrideFramebuffer() { mState.Pop<EStateId::OVERRIDE_FRAMEBUFFER>(); }
  void ResetOverrideFramebuffer() { mState.Reset<EStateId::OVERRIDE_FRAMEBUFFER>(); }
  void Blit();

  // Viewport
  void SetViewport(const Recti& inViewport) { mState.GetCurrent<EStateId::VIEWPORT>() = inViewport; }
  const Recti& GetViewport() const { return mState.GetCurrent<EStateId::VIEWPORT>(); }
  void PushViewport() { mState.PushTop<EStateId::VIEWPORT>(); }
  void PopViewport() { mState.Pop<EStateId::VIEWPORT>(); }
  void ResetViewport() { mState.Reset<EStateId::VIEWPORT>(); }

  // State
  using StateTupleOfStacks = TupleOfStacks<EStateId,
      std::shared_ptr<ShaderProgram>, // EStateId::OVERRIDE_SHADER_PROGRAM
      std::shared_ptr<RenderTarget>,  // EStateId::OVERRIDE_RENDER_TARGET
      std::shared_ptr<Framebuffer>,   // EStateId::OVERRIDE_FRAMEBUFFER
      Recti,                          // EStateId::VIEWPORT
      GL::EDepthFunc,                 // EStateId::DEPTH_FUNC
      bool,                           // EStateId::DEPTH_WRITE_ENABLED
      bool,                           // EStateId::BLEND_ENABLED
      GL::EBlendFactor,               // EStateId::BLEND_SOURCE_FACTOR
      GL::EBlendFactor,               // EStateId::BLEND_DEST_FACTOR
      float,                          // EStateId::POINT_SIZE
      float>;                         // EStateId::LINE_WIDTH
  using State = RendererStateStacks<Renderer, StateTupleOfStacks>;
  friend class RendererStateStacks<Renderer, StateTupleOfStacks>;

  virtual void PushState();
  virtual void PopState();
  void ResetState();
  State& GetState() { return mState; }
  const State& GetState() const { return mState; }

protected:
  // Shader
  void SetShaderProgram(const std::shared_ptr<ShaderProgram>& inShaderProgram) { mShaderProgram = inShaderProgram; }
  std::shared_ptr<ShaderProgram>& GetShaderProgram() { return mShaderProgram; }
  const std::shared_ptr<ShaderProgram>& GetShaderProgram() const { return mShaderProgram; }

  // Framebuffer
  void BindFramebuffer();

  // Draw helpers
  virtual void AdaptToWindow(const Window& inWindow);
  void DrawMesh(const Mesh& inMesh, const Renderer::EDrawType inDrawType = Renderer::EDrawType::SOLID);
  void DrawMesh(const MeshDrawData& inMeshDrawData, const Renderer::EDrawType inDrawType = Renderer::EDrawType::SOLID);
  void DrawVAOElements(const VAO& inVAO,
      const GL::Size inNumberOfElementsToDraw,
      const GL::EPrimitivesType inPrimitivesType = GL::EPrimitivesType::TRIANGLES);
  void DrawVAOArrays(const VAO& inVAO,
      const GL::Size inNumberOfPrimitivesToDraw,
      const GL::EPrimitivesType inPrimitivesType = GL::EPrimitivesType::TRIANGLES,
      const GL::Size inBeginPrimitiveIndex = 0);
  void DrawVAOArraysOrElements(const VAO& inVAO,
      const GL::Size inNumberOfElementsToDraw,
      const GL::EPrimitivesType inPrimitivesType,
      const bool inDrawArrays,
      const GL::Size inBeginArraysPrimitiveIndex);
  template <typename T, std::size_t N>
  void DrawSegmentGeneric(const Segment<T, N>& inSegment);
  template <typename T, std::size_t N>
  void DrawSegmentsGeneric(const Span<Segment<T, N>>& inSegments);
  template <typename T, std::size_t N>
  void DrawPointGeneric(const Vec<T, N>& inPoint);
  template <typename T, std::size_t N>
  void DrawPointsGeneric(const Span<Vec<T, N>>& inPoints);
  template <typename T, std::size_t N>
  void DrawLineStripGeneric(const Span<Vec<T, N>>& inLinePoints);

  // DrawSetup
  class DrawSetup
  {
  public:
    virtual ~DrawSetup() = default;
    std::shared_ptr<ShaderProgram> mShaderProgram;

  private:
    ShaderProgram::GLGuardType mShaderProgramGuard;
    Framebuffer::GLGuardType mFramebufferGuard;
    GLViewportGuard mViewportGuard;
    GLDepthFuncGuard mDepthFuncGuard;
    GLDepthMaskGuard mDepthMaskGuard;
    GLEnableGuard<GL::EEnablable::BLEND> mBlendEnabledGuard;
    GLBlendFuncGuard mBlendFuncGuard;
    GLPointSizeGuard mPointSizeGuard;
    GLLineWidthGuard mLineWidthGuard;
  };

  // State functions
  virtual void PushAllDefaultStateValues();

  // DrawSetup functions
  virtual std::unique_ptr<DrawSetup> CreateDrawSetup() const = 0;
  [[nodiscard]] std::unique_ptr<DrawSetup> PrepareForDraw();
  virtual void PrepareForDraw(DrawSetup& ioDrawSetup);

private:
  // Static resources
  static bool sStaticResourcesInited;

  // State
  State mState { *this };

  // Normal shader (non-override)
  std::shared_ptr<ShaderProgram> mShaderProgram;

  // Render texture
  std::shared_ptr<Framebuffer> mDefaultFramebuffer;
  std::shared_ptr<RenderTarget> mDefaultRenderTarget;

  // State functions
  template <Renderer::EStateId StateId>
  static void ApplyState(const State::ValueType<StateId>& inValue, State& ioState);

  template <Renderer::EStateId StateId>
  static State::ValueType<StateId> GetDefaultValue();
};

// clang-format off
template <typename T> struct RendererFromEStateId { static_assert(!std::is_same_v<T, T>); using Type = void; };
template <> struct RendererFromEStateId<Renderer::EStateId> { using Type = Renderer; };
template <typename T> using RendererFromEStateId_t = typename RendererFromEStateId<T>::Type;
// clang-format on

// Renderer state guards
template <auto StateId>
class RendererStateGuard final
{
public:
  using TRenderer = RendererFromEStateId_t<std::remove_pointer_t<std::decay_t<decltype(StateId)>>>;

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

#define RENDERER_STATE_GUARD_ALL(RENDERER_OBJECT)                                                                      \
  RendererStateGuardAll ANONYMOUS_VARIABLE_NAME() { RENDERER_OBJECT };                                                 \
  UNUSED(ANONYMOUS_VARIABLE_NAME());
#define RENDERER_STATE_GUARD(RENDERER_OBJECT, STATE_ID)                                                                \
  RendererStateGuard<STATE_ID> ANONYMOUS_VARIABLE_NAME() { RENDERER_OBJECT };                                          \
  UNUSED(ANONYMOUS_VARIABLE_NAME());
}

#include "ezgl/Renderer.tcc"