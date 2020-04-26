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

  // Init render/depth_stencil textures and framebuffer
  mDefaultRenderTexture = std::make_shared<Texture2D>(1, 1, GL::ETextureInternalFormat::RGBA8);
  mDefaultDepthStencilTexture = std::make_shared<Texture2D>(1, 1, GL::ETextureInternalFormat::DEPTH24_STENCIL8);
  mDefaultFramebuffer = std::make_shared<Framebuffer>();
  mDefaultFramebuffer->SetAttachment(GL::EFramebufferAttachment::DEPTH_STENCIL_ATTACHMENT, mDefaultDepthStencilTexture);
  mDefaultFramebuffer->CheckFramebufferIsComplete();
}

void Renderer::ClearBackground(const Color4f& inClearColor)
{
  GL::ClearColor(inClearColor);
  GL::Clear(GL::EBufferBitFlags::COLOR);
}

void Renderer::ClearDepth(const float inClearDepth)
{
  glClearDepth(inClearDepth);
  GL::Clear(GL::EBufferBitFlags::DEPTH);
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

void Renderer::SetOverrideRenderTexture(const std::shared_ptr<Texture2D>& inOverrideRenderTexture)
{
  mState.GetCurrent<Renderer::EStateId::OVERRIDE_RENDER_TEXTURE>() = inOverrideRenderTexture;
}

void Renderer::SetOverrideFramebuffer(const std::shared_ptr<Framebuffer>& inOverrideFramebuffer)
{
  mState.GetCurrent<Renderer::EStateId::OVERRIDE_FRAMEBUFFER>() = inOverrideFramebuffer;
}

void Renderer::Blit()
{
  TextureOperations::DrawFullScreenTexture(*GetRenderTexture(), *mDefaultDepthStencilTexture, 0.0f);
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
  mDefaultRenderTexture->Resize(inWindow.GetSize());
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
  std::unique_ptr<Renderer::DrawSetup> draw_setup_ptr = CreateDrawSetup();
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
  const auto framebuffer = GetFramebuffer();
  assert(framebuffer);
  framebuffer->Bind();

  const auto render_texture_to_use = GetRenderTexture();
  framebuffer->SetAttachment(GL::EFramebufferAttachment::COLOR_ATTACHMENT0, render_texture_to_use);
  framebuffer->Resize(render_texture_to_use->GetSize());

  mState.ApplyCurrentState();
}
}