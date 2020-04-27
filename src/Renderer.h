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
class ShaderProgram;
class Texture2D;

class Renderer
{
public:
  enum class EStateId
  {
    OVERRIDE_SHADER_PROGRAM,
    OVERRIDE_RENDER_TEXTURE,
    OVERRIDE_FRAMEBUFFER,
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
  bool GetDepthTestEnabled() const { return mState.GetCurrent<EStateId::DEPTH_ENABLED>(); }
  void PushDepthTestEnabled() { mState.PushTop<EStateId::DEPTH_ENABLED>(); }
  void PopDepthTestEnabled() { mState.Pop<EStateId::DEPTH_ENABLED>(); }
  void ResetDepthEnabled() { mState.Reset<EStateId::DEPTH_ENABLED>(); }

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
  std::shared_ptr<Texture2D> GetDefaultRenderTexture() { return mDefaultRenderTexture; }
  std::shared_ptr<const Texture2D> GetDefaultRenderTexture() const { return mDefaultRenderTexture; }
  std::shared_ptr<Texture2D> GetRenderTexture()
  {
    return GetOverrideRenderTexture() ? GetOverrideRenderTexture() : GetDefaultRenderTexture();
  }
  std::shared_ptr<const Texture2D> GetRenderTexture() const
  {
    return GetOverrideRenderTexture() ? GetOverrideRenderTexture() : GetDefaultRenderTexture();
  }
  void SetOverrideRenderTexture(const std::shared_ptr<Texture2D>& inOverrideRenderTexture);
  std::shared_ptr<Texture2D> GetOverrideRenderTexture()
  {
    return mState.GetCurrent<EStateId::OVERRIDE_RENDER_TEXTURE>();
  }
  std::shared_ptr<const Texture2D> GetOverrideRenderTexture() const
  {
    return mState.GetCurrent<EStateId::OVERRIDE_RENDER_TEXTURE>();
  }
  void PushOverrideRenderTexture() { mState.PushTop<EStateId::OVERRIDE_RENDER_TEXTURE>(); }
  void PopOverrideRenderTexture() { mState.Pop<EStateId::OVERRIDE_RENDER_TEXTURE>(); }
  void ResetOverrideRenderTexture() { mState.Reset<EStateId::OVERRIDE_RENDER_TEXTURE>(); }

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

  // State
  using StateTupleOfStacks = TupleOfStacks<EStateId,
      std::shared_ptr<ShaderProgram>, // EStateId::OVERRIDE_SHADER_PROGRAM
      std::shared_ptr<Texture2D>,     // EStateId::OVERRIDE_RENDER_TEXTURE
      std::shared_ptr<Framebuffer>,   // EStateId::OVERRIDE_FRAMEBUFFER
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
  // Shader
  void SetShaderProgram(const std::shared_ptr<ShaderProgram>& inShaderProgram) { mShaderProgram = inShaderProgram; }
  std::shared_ptr<ShaderProgram>& GetShaderProgram() { return mShaderProgram; }
  const std::shared_ptr<ShaderProgram>& GetShaderProgram() const { return mShaderProgram; }

  void BindFramebuffer();

  // Draw helpers
  virtual void Begin(const Window& inWindow);
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

  // Helpers or common functionality
  class DrawSetup
  {
  public:
    std::shared_ptr<ShaderProgram> mShaderProgram;

  private:
    GLMultiGuard<ShaderProgram::GLGuardType,
        Framebuffer::GLGuardType,
        GLGuardWrap_t<GL::EEnablable::DEPTH_TEST>,
        GLGuardWrap_t<GL::EEnablable::BLEND>>
        mGuard;
  };
  virtual std::unique_ptr<DrawSetup> CreateDrawSetup() const { return std::make_unique<DrawSetup>(); }
  [[nodiscard]] std::unique_ptr<DrawSetup> PrepareForDraw();
  virtual void PrepareForDraw(DrawSetup& ioDrawSetup);

private:
  // Static resources
  static bool sStaticResourcesInited;

  // State
  State mState { *this };

  template <Renderer::EStateId StateId>
  static void ApplyState(const State::ValueType<StateId>& inValue, State& ioState);

  template <Renderer::EStateId StateId>
  static State::ValueType<StateId> GetDefaultValue();

  // Normal shader (non-override)
  std::shared_ptr<ShaderProgram> mShaderProgram;

  // Render texture
  std::shared_ptr<Framebuffer> mDefaultFramebuffer;
  std::shared_ptr<Texture2D> mDefaultRenderTexture;
  std::shared_ptr<Texture2D> mDefaultDepthStencilTexture;
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