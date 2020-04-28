#include "Renderer.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "FileUtils.h"
#include "GL.h"
#include "GLGuard.h"
#include "GLTypeTraits.h"
#include "Geometry.h"
#include "Math.h"
#include "MeshDrawData.h"
#include "MeshFactory.h"
#include "PointLight.h"
#include "ShaderProgram.h"
#include "TextureFactory.h"
#include "TextureOperations.h"
#include "UBO.h"
#include "Window.h"

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
  mDefaultFramebuffer = std::make_shared<Framebuffer>();
}

void Renderer::ClearBackground(const Color4f& inClearColor)
{
  const GLBindGuard<GL::EBindingType::FRAMEBUFFER> framebuffer_bind_guard;
  BindFramebuffer();

  GL::ClearColor(inClearColor);
}

void Renderer::ClearDepth(const float inClearDepth)
{
  const GLBindGuard<GL::EBindingType::FRAMEBUFFER> framebuffer_bind_guard;
  BindFramebuffer();

  GL::ClearDepth(inClearDepth);
}

void Renderer::Clear(const Color4f& inClearColor, const float inClearDepth)
{
  ClearBackground(inClearColor);
  ClearDepth(inClearDepth);
}

void Renderer::SetDepthTestEnabled(const bool inDepthTestEnabled)
{
  mState.GetCurrent<Renderer::EStateId::DEPTH_ENABLED>() = inDepthTestEnabled;
}

void Renderer::SetBlendEnabled(const bool inBlendEnabled)
{
  mState.GetCurrent<Renderer::EStateId::BLEND_ENABLED>() = inBlendEnabled;
}

void Renderer::SetBlendFunc(const GL::EBlendFactor inBlendSourceFactor, const GL::EBlendFactor inBlendDestFactor)
{
  mState.GetCurrent<Renderer::EStateId::BLEND_SOURCE_FACTOR>() = inBlendSourceFactor;
  mState.GetCurrent<Renderer::EStateId::BLEND_DEST_FACTOR>() = inBlendDestFactor;
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

void Renderer::SetOverrideFramebuffer(const std::shared_ptr<Framebuffer>& inOverrideFramebuffer)
{
  mState.GetCurrent<Renderer::EStateId::OVERRIDE_FRAMEBUFFER>() = inOverrideFramebuffer;
}

void Renderer::BindFramebuffer()
{
  auto framebuffer = GetFramebuffer();
  framebuffer->Bind();

  const auto render_target_to_use = GetRenderTarget();
  framebuffer->SetAttachment(GL::EFramebufferAttachment::COLOR_ATTACHMENT0, render_target_to_use->GetColorTexture());
  if (GL::IsDepthOnlyFormat(render_target_to_use->GetDepthTexture()->GetFormat()))
  {
    framebuffer->SetAttachment(GL::EFramebufferAttachment::DEPTH_STENCIL_ATTACHMENT,
        render_target_to_use->GetDepthTexture());
  }
  else
  {
    framebuffer->SetAttachment(GL::EFramebufferAttachment::DEPTH_ATTACHMENT, render_target_to_use->GetDepthTexture());
  }
  framebuffer->Resize(render_target_to_use->GetColorTexture()->GetSize());
  framebuffer->CheckFramebufferIsComplete();
}

void Renderer::Blit()
{
  TextureOperations::DrawFullScreenTexture(*GetRenderTarget()->GetColorTexture(),
      *GetRenderTarget()->GetDepthTexture());
}

void Renderer::PushState() { mState.PushAllTops(); }
void Renderer::PopState() { mState.PopAll(); }

void Renderer::ResetState()
{
  mState.PopAll();
  mState.PushAllDefaultValues();
  mState.ApplyCurrentState();
}

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

void Renderer::Begin(const Window& inWindow)
{
  ResetState();
  GL::Viewport(Zero<Vec2i>(), inWindow.GetFramebufferSize());
  mDefaultRenderTarget->Resize(inWindow.GetSize());
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

  // Prepare framebuffer
  BindFramebuffer();

  mState.ApplyCurrentState();
}
}