#include "ez/RendererGPU.h"
#include "ez/Camera.h"
#include "ez/DirectionalLight.h"
#include "ez/FileUtils.h"
#include "ez/GL.h"
#include "ez/GLGuard.h"
#include "ez/GLTypeTraits.h"
#include "ez/Geometry.h"
#include "ez/Math.h"
#include "ez/MeshDrawData.h"
#include "ez/MeshFactory.h"
#include "ez/PointLight.h"
#include "ez/ShaderProgram.h"
#include "ez/TextureFactory.h"
#include "ez/TextureOperations.h"
#include "ez/UBO.h"
#include "ez/Window.h"

namespace ez
{
RendererGPU::RendererGPU()
{
  // Init default render target and default framebuffer
  mDefaultRenderTarget
      = std::make_shared<RenderTarget>(GL::ETextureFormat::RGBA8, GL::ETextureFormat::DEPTH24_STENCIL8);

  // Renderer leaf subclasses must call PushAllDefaultValues() in their constructor...!
}

void RendererGPU::ClearBackground(const Color4f& inClearColor)
{
  const RenderTarget::GLGuardType framebuffer_bind_guard;
  BindRenderTarget();

  GL::ClearColor(inClearColor);
}

void RendererGPU::ClearDepth(const float inClearDepth)
{
  const RenderTarget::GLGuardType framebuffer_bind_guard;
  BindRenderTarget();

  GL::ClearDepth(inClearDepth);
}

void RendererGPU::Clear(const Color4f& inClearColor, const float inClearDepth)
{
  ClearBackground(inClearColor);
  ClearDepth(inClearDepth);
}

void RendererGPU::SetDepthWriteEnabled(const bool inDepthWriteEnabled)
{
  mState.GetCurrent<EStateId::DEPTH_WRITE_ENABLED>() = inDepthWriteEnabled;
}

void RendererGPU::SetBlendEnabled(const bool inBlendEnabled)
{
  mState.GetCurrent<EStateId::BLEND_ENABLED>() = inBlendEnabled;
}

void RendererGPU::SetBlendFunc(const GL::EBlendFactor inBlendSourceFactor, const GL::EBlendFactor inBlendDestFactor)
{
  SetBlendFuncRGB(inBlendSourceFactor, inBlendDestFactor);
  SetBlendFuncAlpha(inBlendSourceFactor, inBlendDestFactor);
}

void RendererGPU::SetBlendFuncRGB(const GL::EBlendFactor inBlendSourceFactorRGB,
    const GL::EBlendFactor inBlendDestFactorRGB)
{
  mState.GetCurrent<EStateId::BLEND_FACTORS>().mSourceBlendFactorRGB = inBlendSourceFactorRGB;
  mState.GetCurrent<EStateId::BLEND_FACTORS>().mDestBlendFactorRGB = inBlendDestFactorRGB;
}

void RendererGPU::SetBlendFuncAlpha(const GL::EBlendFactor inBlendSourceFactorAlpha,
    const GL::EBlendFactor inBlendDestFactorAlpha)
{
  mState.GetCurrent<EStateId::BLEND_FACTORS>().mSourceBlendFactorAlpha = inBlendSourceFactorAlpha;
  mState.GetCurrent<EStateId::BLEND_FACTORS>().mDestBlendFactorAlpha = inBlendDestFactorAlpha;
}

GL::EBlendFactor RendererGPU::GetBlendSourceFactorRGB() const
{
  return mState.GetCurrent<EStateId::BLEND_FACTORS>().mSourceBlendFactorRGB;
}

GL::EBlendFactor RendererGPU::GetBlendDestFactorRGB() const
{
  return mState.GetCurrent<EStateId::BLEND_FACTORS>().mDestBlendFactorRGB;
}

GL::EBlendFactor RendererGPU::GetBlendSourceFactorAlpha() const
{
  return mState.GetCurrent<EStateId::BLEND_FACTORS>().mSourceBlendFactorAlpha;
}

GL::EBlendFactor RendererGPU::GetBlendDestFactorAlpha() const
{
  return mState.GetCurrent<EStateId::BLEND_FACTORS>().mDestBlendFactorAlpha;
}

void RendererGPU::SetPointSize(const float inPointSize) { mState.GetCurrent<EStateId::POINT_SIZE>() = inPointSize; }

void RendererGPU::SetLineWidth(const float inLineWidth) { mState.GetCurrent<EStateId::LINE_WIDTH>() = inLineWidth; }

void RendererGPU::SetOverrideShaderProgram(const std::shared_ptr<ShaderProgram>& inShaderProgram)
{
  mState.GetCurrent<EStateId::OVERRIDE_SHADER_PROGRAM>() = inShaderProgram;
}

void RendererGPU::SetOverrideRenderTarget(const std::shared_ptr<RenderTarget>& inOverrideRenderTarget)
{
  mState.GetCurrent<EStateId::OVERRIDE_RENDER_TARGET>() = inOverrideRenderTarget;
}

void RendererGPU::BindRenderTarget() { GetRenderTarget()->Bind(); }

void RendererGPU::Blit()
{
  TextureOperations::DrawFullScreenTexture(*GetRenderTarget()->GetColorTexture(),
      *GetRenderTarget()->GetDepthTexture());
}

void RendererGPU::Blit(RenderTarget& ioRenderTarget)
{
  const RenderTarget::GLGuardType render_target_guard;

  ioRenderTarget.Bind();
  Blit();
}

void RendererGPU::PushState() { mState.PushAllTops(); }

void RendererGPU::PopState() { mState.PopAll(); }

void RendererGPU::ResetState()
{
  PopState();
  PushAllDefaultStateValues();
}

void RendererGPU::PushAllDefaultStateValues() { mState.PushAllDefaultValues(); }

void RendererGPU::DrawCustom(const std::function<void()>& inCustomDrawFunction)
{
  const auto draw_setup = PrepareForDraw();
  inCustomDrawFunction();
}

void RendererGPU::DrawMesh(const Mesh& inMesh, const RendererGPU::EDrawType inDrawType)
{
  const auto mesh_draw_data = MeshDrawData { inMesh };
  DrawMesh(mesh_draw_data, inDrawType);
}
void RendererGPU::DrawMesh(const MeshDrawData& inMeshDrawData, const RendererGPU::EDrawType inDrawType)
{
  if (inDrawType == EDrawType::WIREFRAME)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  const auto primitives_type
      = (inDrawType == EDrawType::POINTS) ? GL::EPrimitivesType::POINTS : GL::EPrimitivesType::TRIANGLES;

  DrawVAOElements(inMeshDrawData.GetVAO(), inMeshDrawData.GetNumberOfElements(), primitives_type);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // TODO: Restore this properly
}

void RendererGPU::AdaptToWindow(const Window& inWindow)
{
  SetViewport(AARecti(Zero<Vec2i>(), inWindow.GetFramebufferSize()));
  GetRenderTarget()->Resize(inWindow.GetFramebufferSize());
}

void RendererGPU::DrawVAOArraysOrElements(const VAO& inVAO,
    const GL::Size inNumberOfElementsToDraw,
    const GL::EPrimitivesType inPrimitivesType,
    const bool inDrawArrays,
    const GL::Size inBeginArraysPrimitiveIndex)
{
  const auto draw_setup = PrepareForDraw();
  const auto vao_bind_guard = inVAO.BindGuarded();

  if (inDrawArrays)
  {
    GL::DrawArrays(inPrimitivesType, inNumberOfElementsToDraw, inBeginArraysPrimitiveIndex);
  }
  else
  {
    static constexpr auto ElementIdType = GLTypeTraits<Mesh::VertexId>::GLType;
    GL::DrawElements(inPrimitivesType, inNumberOfElementsToDraw, ElementIdType);
  }
}

void RendererGPU::DrawVAOElements(const VAO& inVAO,
    const GL::Size inNumberOfElementsToDraw,
    const GL::EPrimitivesType inPrimitivesType)
{
  constexpr auto draw_arrays = false;
  DrawVAOArraysOrElements(inVAO, inNumberOfElementsToDraw, inPrimitivesType, draw_arrays, 0);
}

void RendererGPU::DrawVAOArrays(const VAO& inVAO,
    const GL::Size inNumberOfPrimitivesToDraw,
    const GL::EPrimitivesType inPrimitivesType,
    const GL::Size inBeginPrimitiveIndex)
{
  constexpr auto draw_arrays = true;
  DrawVAOArraysOrElements(inVAO, inNumberOfPrimitivesToDraw, inPrimitivesType, draw_arrays, inBeginPrimitiveIndex);
}

std::unique_ptr<RendererGPU::DrawSetup> RendererGPU::PrepareForDraw()
{
  std::unique_ptr<DrawSetup> draw_setup_ptr = CreateDrawSetup();
  PrepareForDraw(*draw_setup_ptr);
  return draw_setup_ptr;
}

void RendererGPU::PrepareForDraw(DrawSetup& ioDrawSetup)
{
  // Prepare shader program
  const auto override_shader_program = GetOverrideShaderProgram();
  ioDrawSetup.mShaderProgram = (override_shader_program ? override_shader_program : GetShaderProgram());
  if (ioDrawSetup.mShaderProgram)
    ioDrawSetup.mShaderProgram->Bind();

  // Prepare RenderTarget (if any)
  BindRenderTarget();

  mState.ApplyCurrentState();
}
}