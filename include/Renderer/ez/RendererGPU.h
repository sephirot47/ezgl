#pragma once

#include "ez/AARect.h"
#include "ez/Camera.h"
#include "ez/Color.h"
#include "ez/DirectionalLight.h"
#include "ez/Framebuffer.h"
#include "ez/Macros.h"
#include "ez/Material3D.h"
#include "ez/Math.h"
#include "ez/MeshDrawData.h"
#include "ez/OrthographicCamera.h"
#include "ez/PerspectiveCamera.h"
#include "ez/Plane.h"
#include "ez/PointLight.h"
#include "ez/RenderTarget.h"
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
#include <type_traits>

namespace ez
{
class MeshDrawData;
class ShaderProgram;
class Texture2D;

template <auto StateId>
class RendererStateGuard;

class RendererGPU : public Renderer
{
public:
  enum class EStateId
  {
    OVERRIDE_SHADER_PROGRAM,
    OVERRIDE_RENDER_TARGET,
    VIEWPORT,
    DEPTH_FUNC,
    DEPTH_WRITE_ENABLED,
    BLEND_ENABLED,
    BLEND_FACTORS,
    BLEND_COLOR,
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
  RendererGPU();
  RendererGPU(const RendererGPU& inRHS) = default;
  RendererGPU& operator=(const RendererGPU& inRHS) = default;
  RendererGPU(RendererGPU&&) = default;
  RendererGPU& operator=(RendererGPU&& inRHS) = default;
  virtual ~RendererGPU() = default;

public:
  // Clears
  void ClearBackground(const Color4f& inClearColor);
  void ClearDepth(const float inClearDepth = 1.0f);
  void Clear(const Color4f& inClearColor = Black<Color4f>(), const float inClearDepth = 1.0f);

  // Depth func
  void SetDepthFunc(const GL::EDepthFunc inDepthFunc) { mState.GetCurrent<EStateId::DEPTH_FUNC>() = inDepthFunc; }
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
  void SetBlendFuncRGB(const GL::EBlendFactor inBlendSourceFactorRGB, const GL::EBlendFactor inBlendDestFactorRGB);
  void SetBlendFuncAlpha(const GL::EBlendFactor inBlendSourceFactorAlpha,
      const GL::EBlendFactor inBlendDestFactorAlpha);
  GL::EBlendFactor GetBlendSourceFactorRGB() const;
  GL::EBlendFactor GetBlendDestFactorRGB() const;
  GL::EBlendFactor GetBlendSourceFactorAlpha() const;
  GL::EBlendFactor GetBlendDestFactorAlpha() const;
  void PushBlendFactors() { mState.PushTop<EStateId::BLEND_FACTORS>(); }
  void PopBlendFactors() { mState.Pop<EStateId::BLEND_FACTORS>(); }
  void ResetBlendFactors() { mState.Reset<EStateId::BLEND_FACTORS>(); }

  // Blend color
  void SetBlendColor(const Color4f& inBlendColor) { mState.GetCurrent<EStateId::BLEND_COLOR>() = inBlendColor; }
  Color4f GetBlendColor() const { return mState.GetCurrent<EStateId::BLEND_COLOR>(); }
  void PushBlendColor() { mState.PushTop<EStateId::BLEND_COLOR>(); }
  void PopBlendColor() { mState.Pop<EStateId::BLEND_COLOR>(); }
  void ResetBlendColor() { mState.Reset<EStateId::BLEND_COLOR>(); }

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
  void Blit();
  void Blit(RenderTarget& ioRenderTarget);

  // Viewport
  void SetViewport(const AARecti& inViewport) { mState.GetCurrent<EStateId::VIEWPORT>() = inViewport; }
  const AARecti& GetViewport() const { return mState.GetCurrent<EStateId::VIEWPORT>(); }
  void PushViewport() { mState.PushTop<EStateId::VIEWPORT>(); }
  void PopViewport() { mState.Pop<EStateId::VIEWPORT>(); }
  void ResetViewport() { mState.Reset<EStateId::VIEWPORT>(); }

  // State
  using StateTupleOfStacks = TupleOfStacks<EStateId,
      std::shared_ptr<ShaderProgram>, // EStateId::OVERRIDE_SHADER_PROGRAM
      std::shared_ptr<RenderTarget>,  // EStateId::OVERRIDE_RENDER_TARGET
      AARecti,                        // EStateId::VIEWPORT
      GL::EDepthFunc,                 // EStateId::DEPTH_FUNC
      bool,                           // EStateId::DEPTH_WRITE_ENABLED
      bool,                           // EStateId::BLEND_ENABLED
      GL::BlendFactors,               // EStateId::BLEND_FACTORS
      Color4f,                        // EStateId::BLEND_COLOR
      float,                          // EStateId::POINT_SIZE
      float>;                         // EStateId::LINE_WIDTH
  using State = RendererStateStacks<RendererGPU, StateTupleOfStacks>;
  friend class RendererStateStacks<RendererGPU, StateTupleOfStacks>;

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

  // RenderTarget
  void BindRenderTarget();

  // Draw helpers
  virtual void AdaptToWindow(const Window& inWindow);
  void DrawCustom(const std::function<void()>& inCustomDrawFunction);
  void DrawMesh(const Mesh& inMesh, const RendererGPU::EDrawType inDrawType = RendererGPU::EDrawType::SOLID);
  void DrawMesh(const MeshDrawData& inMeshDrawData,
      const RendererGPU::EDrawType inDrawType = RendererGPU::EDrawType::SOLID);
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
  void DrawCircleSectionGeneric(const AngleRads inAngle, std::size_t inNumVertices);
  template <typename T, std::size_t N>
  void DrawCircleSectionBoundaryGeneric(const AngleRads inAngle, std::size_t inNumVertices);
  template <typename T, std::size_t N>
  void DrawTrianglesGeneric(const Span<Triangle<T, N>>& inTriangles);
  template <typename T>
  void DrawAARectsGeneric(const Span<AARect<T>>& inAARects);
  template <typename T, std::size_t N>
  void DrawSegmentsGeneric(const Span<Segment<T, N>>& inSegments);
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
  std::shared_ptr<RenderTarget> mDefaultRenderTarget;

  // State functions
  template <RendererGPU::EStateId StateId>
  static void ApplyState(const State::ValueType<StateId>& inValue, State& ioState);

  template <RendererGPU::EStateId StateId>
  static State::ValueType<StateId> GetDefaultValue();
};

template <>
struct RendererFromEStateId<RendererGPU::EStateId>
{
  using Type = RendererGPU;
};
}

#include "ez/RendererGPU.tcc"