#include "ezgl/Renderer.h"
#include "ezgl/Camera.h"
#include "ezgl/DirectionalLight.h"
#include "ezgl/FileUtils.h"
#include "ezgl/GL.h"
#include "ezgl/GLGuard.h"
#include "ezgl/GLTypeTraits.h"
#include "ezgl/Geometry.h"
#include "ezgl/Math.h"
#include "ezgl/MeshDrawData.h"
#include "ezgl/MeshFactory.h"
#include "ezgl/PointLight.h"
#include "ezgl/ShaderProgram.h"
#include "ezgl/TextureFactory.h"
#include "ezgl/TextureOperations.h"
#include "ezgl/UBO.h"
#include "ezgl/Window.h"

namespace egl
{
bool Renderer::sStaticResourcesInited = false;

Renderer::Renderer()
{
  // Init static resources
  if (!sStaticResourcesInited)
  {
    sStaticResourcesInited = true;
  }

  // Init default render target and default framebuffer
  mDefaultRenderTarget
      = std::make_shared<RenderTarget>(GL::ETextureFormat::RGBA8, GL::ETextureFormat::DEPTH24_STENCIL8);

  // Renderer subclasses must call PushAllDefaultValues() in their constructor...!
}

void Renderer::ClearBackground(const Color4f& inClearColor)
{
  const RenderTarget::GLGuardType framebuffer_bind_guard;
  BindRenderTarget();

  GL::ClearColor(inClearColor);
}

void Renderer::ClearDepth(const float inClearDepth)
{
  const RenderTarget::GLGuardType framebuffer_bind_guard;
  BindRenderTarget();

  GL::ClearDepth(inClearDepth);
}

void Renderer::Clear(const Color4f& inClearColor, const float inClearDepth)
{
  ClearBackground(inClearColor);
  ClearDepth(inClearDepth);
}

void Renderer::SetBlendEnabled(const bool inBlendEnabled)
{
  mState.GetCurrent<Renderer::EStateId::BLEND_ENABLED>() = inBlendEnabled;
}

void Renderer::SetBlendFunc(const GL::EBlendFactor inBlendSourceFactor, const GL::EBlendFactor inBlendDestFactor)
{
  SetBlendFuncRGB(inBlendSourceFactor, inBlendDestFactor);
  SetBlendFuncAlpha(inBlendSourceFactor, inBlendDestFactor);
}

void Renderer::SetBlendFuncRGB(const GL::EBlendFactor inBlendSourceFactorRGB, const GL::EBlendFactor inBlendDestFactorRGB)
{
  mState.GetCurrent<Renderer::EStateId::BLEND_FACTORS>().mSourceBlendFactorRGB = inBlendSourceFactorRGB;
  mState.GetCurrent<Renderer::EStateId::BLEND_FACTORS>().mDestBlendFactorRGB = inBlendDestFactorRGB;
}

void Renderer::SetBlendFuncAlpha(const GL::EBlendFactor inBlendSourceFactorAlpha,
    const GL::EBlendFactor inBlendDestFactorAlpha)
{
  mState.GetCurrent<Renderer::EStateId::BLEND_FACTORS>().mSourceBlendFactorAlpha = inBlendSourceFactorAlpha;
  mState.GetCurrent<Renderer::EStateId::BLEND_FACTORS>().mDestBlendFactorAlpha = inBlendDestFactorAlpha;
}

GL::EBlendFactor Renderer::GetBlendSourceFactorRGB() const
{
  return mState.GetCurrent<EStateId::BLEND_FACTORS>().mSourceBlendFactorRGB;
}

GL::EBlendFactor Renderer::GetBlendDestFactorRGB() const
{
  return mState.GetCurrent<EStateId::BLEND_FACTORS>().mDestBlendFactorRGB;
}

GL::EBlendFactor Renderer::GetBlendSourceFactorAlpha() const
{
  return mState.GetCurrent<EStateId::BLEND_FACTORS>().mSourceBlendFactorAlpha;
}

GL::EBlendFactor Renderer::GetBlendDestFactorAlpha() const
{
  return mState.GetCurrent<EStateId::BLEND_FACTORS>().mDestBlendFactorAlpha;
}

void Renderer::SetPointSize(const float inPointSize)
{
  mState.GetCurrent<Renderer::EStateId::POINT_SIZE>() = inPointSize;
}

void Renderer::SetLineWidth(const float inLineWidth)
{
  mState.GetCurrent<Renderer::EStateId::LINE_WIDTH>() = inLineWidth;
}

void Renderer::SetOverrideShaderProgram(const std::shared_ptr<ShaderProgram>& inShaderProgram)
{
  mState.GetCurrent<Renderer::EStateId::OVERRIDE_SHADER_PROGRAM>() = inShaderProgram;
}

void Renderer::SetOverrideRenderTarget(const std::shared_ptr<RenderTarget>& inOverrideRenderTarget)
{
  mState.GetCurrent<Renderer::EStateId::OVERRIDE_RENDER_TARGET>() = inOverrideRenderTarget;
}

void Renderer::BindRenderTarget() { GetRenderTarget()->Bind(); }

void Renderer::Blit()
{
  TextureOperations::DrawFullScreenTexture(*GetRenderTarget()->GetColorTexture(),
      *GetRenderTarget()->GetDepthTexture());
}

void Renderer::Blit(RenderTarget& ioRenderTarget)
{
  const RenderTarget::GLGuardType render_target_guard;

  ioRenderTarget.Bind();
  Blit();
}

void Renderer::PushState() { mState.PushAllTops(); }

void Renderer::PopState() { mState.PopAll(); }

void Renderer::ResetState()
{
  PopState();
  PushAllDefaultStateValues();
}

void Renderer::PushAllDefaultStateValues() { mState.PushAllDefaultValues(); }

void Renderer::DrawMesh(const Mesh& inMesh, const Renderer::EDrawType inDrawType)
{
  const auto mesh_draw_data = MeshDrawData { inMesh };
  DrawMesh(mesh_draw_data, inDrawType);
}
void Renderer::DrawMesh(const MeshDrawData& inMeshDrawData, const Renderer::EDrawType inDrawType)
{
  if (inDrawType == EDrawType::WIREFRAME)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  const auto primitives_type
      = (inDrawType == EDrawType::POINTS) ? GL::EPrimitivesType::POINTS : GL::EPrimitivesType::TRIANGLES;

  DrawVAOElements(inMeshDrawData.GetVAO(), inMeshDrawData.GetNumberOfElements(), primitives_type);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // TODO: Restore this properly
}

void Renderer::AdaptToWindow(const Window& inWindow)
{
  SetViewport(Recti(Zero<Vec2i>(), inWindow.GetFramebufferSize()));
  GetRenderTarget()->Resize(inWindow.GetFramebufferSize());
}

void Renderer::DrawVAOArraysOrElements(const VAO& inVAO,
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

void Renderer::DrawVAOElements(const VAO& inVAO,
    const GL::Size inNumberOfElementsToDraw,
    const GL::EPrimitivesType inPrimitivesType)
{
  constexpr auto draw_arrays = false;
  DrawVAOArraysOrElements(inVAO, inNumberOfElementsToDraw, inPrimitivesType, draw_arrays, 0);
}

void Renderer::DrawVAOArrays(const VAO& inVAO,
    const GL::Size inNumberOfPrimitivesToDraw,
    const GL::EPrimitivesType inPrimitivesType,
    const GL::Size inBeginPrimitiveIndex)
{
  constexpr auto draw_arrays = false;
  DrawVAOArraysOrElements(inVAO, inNumberOfPrimitivesToDraw, inPrimitivesType, draw_arrays, inBeginPrimitiveIndex);
}

std::unique_ptr<Renderer::DrawSetup> Renderer::PrepareForDraw()
{
  std::unique_ptr<DrawSetup> draw_setup_ptr = CreateDrawSetup();
  PrepareForDraw(*draw_setup_ptr);
  return draw_setup_ptr;
}

void Renderer::PrepareForDraw(DrawSetup& ioDrawSetup)
{
  EXPECTS(GetShaderProgram());

  // Prepare shader program
  const auto override_shader_program = GetOverrideShaderProgram();
  ioDrawSetup.mShaderProgram = (override_shader_program ? override_shader_program : GetShaderProgram());
  assert(ioDrawSetup.mShaderProgram);
  ioDrawSetup.mShaderProgram->Bind();

  // Prepare RenderTarget (if any)
  BindRenderTarget();

  mState.ApplyCurrentState();
}
}