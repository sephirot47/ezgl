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

  // Init render texture
  mRenderTextureFramebuffer = std::make_unique<Framebuffer>();
  mRenderTextureFramebuffer->CreateRenderbuffer(GL::EFramebufferAttachment::DEPTH_STENCIL_ATTACHMENT,
      GL::ETextureInternalFormat::DEPTH24_STENCIL8);
}

void Renderer::ClearBackground(const Color4f& inClearColor)
{
  GL::ClearColor(inClearColor);
  GL::ClearBuffer(GL::EBufferBitFlags::COLOR);
}

void Renderer::ClearDepth(const float inClearDepth)
{
  glClearDepth(inClearDepth);
  GL::ClearBuffer(GL::EBufferBitFlags::DEPTH);
}

void Renderer::Clear(const Color4f& inClearColor, const float inClearDepth)
{
  ClearBackground(inClearColor);
  ClearDepth(inClearDepth);
}

void Renderer::SetDepthTestEnabled(const bool inDepthTestEnabled)
{
  mState.GetCurrent<Renderer::EStateId::DEPTH_ENABLED>() = inDepthTestEnabled;
  GL::SetEnabled(GL::Enablable::DEPTH_TEST, inDepthTestEnabled);
}

void Renderer::SetBlendEnabled(const bool inBlendEnabled)
{
  mState.GetCurrent<Renderer::EStateId::BLEND_ENABLED>() = inBlendEnabled;
  GL::SetEnabled(GL::Enablable::BLEND, inBlendEnabled);
}

void Renderer::SetBlendFunc(const GL::EBlendFactor inBlendSourceFactor, const GL::EBlendFactor inBlendDestFactor)
{
  mState.GetCurrent<Renderer::EStateId::BLEND_SOURCE_FACTOR>() = inBlendSourceFactor;
  mState.GetCurrent<Renderer::EStateId::BLEND_DEST_FACTOR>() = inBlendDestFactor;
  GL::BlendFunc(inBlendSourceFactor, inBlendDestFactor);
}

void Renderer::SetPointSize(const float inPointSize)
{
  mState.GetCurrent<Renderer::EStateId::POINT_SIZE>() = inPointSize;
  GL::PointSize(inPointSize);
}

void Renderer::SetLineWidth(const float inLineWidth)
{
  mState.GetCurrent<Renderer::EStateId::LINE_WIDTH>() = inLineWidth;
  GL::LineWidth(inLineWidth);
}

void Renderer::SetOverrideShaderProgram(const std::shared_ptr<ShaderProgram>& inShaderProgram)
{
  mState.GetCurrent<Renderer::EStateId::OVERRIDE_SHADER_PROGRAM>() = inShaderProgram;
}

void Renderer::SetRenderTexture(const std::shared_ptr<Texture2D>& inRenderTexture)
{
  mRenderTextureFramebuffer->SetAttachment(GL::EFramebufferAttachment::COLOR_ATTACHMENT0, inRenderTexture);
  mState.GetCurrent<Renderer::EStateId::RENDER_TEXTURE>() = inRenderTexture;

  if (inRenderTexture)
  {
    mRenderTextureFramebuffer->Bind(); // Leave it bound so that next draw calls are to this framebuffer
    mRenderTextureFramebuffer->Resize(inRenderTexture->GetSize());
  }
  else
  {
    // If we want no render texture, then unbind framebuffer so that we do not draw into this framebuffer anymore
    if (mRenderTextureFramebuffer->IsBound())
      mRenderTextureFramebuffer->UnBind();
  }
}

void Renderer::SetCamera(const std::shared_ptr<Camera>& inCamera)
{
  mState.GetCurrent<Renderer::EStateId::CAMERA>() = inCamera;
}

std::shared_ptr<Camera> Renderer::GetCamera() { return mState.GetCurrent<Renderer::EStateId::CAMERA>(); }
std::shared_ptr<const Camera> Renderer::GetCamera() const { return const_cast<Renderer&>(*this).GetCamera(); }

std::shared_ptr<PerspectiveCamera> Renderer::GetPerspectiveCamera()
{
  return std::dynamic_pointer_cast<PerspectiveCamera>(mState.GetCurrent<Renderer::EStateId::CAMERA>());
}

std::shared_ptr<const PerspectiveCamera> Renderer::GetPerspectiveCamera() const
{
  return const_cast<Renderer&>(*this).GetPerspectiveCamera();
}

std::shared_ptr<OrthographicCamera> Renderer::GetOrthographicCamera()
{
  return std::dynamic_pointer_cast<OrthographicCamera>(mState.GetCurrent<Renderer::EStateId::CAMERA>());
}

std::shared_ptr<const OrthographicCamera> Renderer::GetOrthographicCamera() const
{
  return const_cast<Renderer&>(*this).GetOrthographicCamera();
}

void Renderer::PushState() { mState.PushAllTops(); }
void Renderer::PopState() { mState.PopAll(); }

void Renderer::ResetState()
{
  mState.PopAll();
  mState.PushAllDefaultValues();
  mState.ApplyCurrentState();
}

void Renderer::DrawMesh(ShaderProgram& ioShaderProgram, const Mesh& inMesh, const Renderer::EDrawType inDrawType)
{
  const auto mesh_draw_data = MeshDrawData { inMesh };
  DrawMesh(ioShaderProgram, mesh_draw_data, inDrawType);
}
void Renderer::DrawMesh(ShaderProgram& ioShaderProgram,
    const MeshDrawData& inMeshDrawData,
    const Renderer::EDrawType inDrawType)
{
  if (inDrawType == EDrawType::WIREFRAME)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  const auto primitives_type
      = (inDrawType == EDrawType::POINTS) ? GL::EPrimitivesType::POINTS : GL::EPrimitivesType::TRIANGLES;

  DrawVAOElements(ioShaderProgram, inMeshDrawData.GetVAO(), inMeshDrawData.GetNumberOfElements(), primitives_type);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // TODO: Restore this properly
}

void Renderer::Begin(const Window& inWindow)
{
  ResetState();
  GL::Viewport(Zero<Vec2i>(), inWindow.GetFramebufferSize());
}

void Renderer::DrawVAOArraysOrElements(ShaderProgram& ioShaderProgram,
    const VAO& inVAO,
    const GL::Size inNumberOfElementsToDraw,
    const GL::EPrimitivesType inPrimitivesType,
    const bool inDrawArrays,
    const GL::Size inBeginArraysPrimitiveIndex)
{
  const auto use_shader_program_bind_guard = UseShaderProgram(ioShaderProgram);

  GL_BIND_GUARD_VAR(inVAO);
  inVAO.Bind();

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

void Renderer::DrawVAOElements(ShaderProgram& ioShaderProgram,
    const VAO& inVAO,
    const GL::Size inNumberOfElementsToDraw,
    const GL::EPrimitivesType inPrimitivesType)
{
  constexpr auto draw_arrays = false;
  DrawVAOArraysOrElements(ioShaderProgram, inVAO, inNumberOfElementsToDraw, inPrimitivesType, draw_arrays, 0);
}

void Renderer::DrawVAOArrays(ShaderProgram& ioShaderProgram,
    const VAO& inVAO,
    const GL::Size inNumberOfPrimitivesToDraw,
    const GL::EPrimitivesType inPrimitivesType,
    const GL::Size inBeginPrimitiveIndex)
{
  constexpr auto draw_arrays = false;
  DrawVAOArraysOrElements(ioShaderProgram,
      inVAO,
      inNumberOfPrimitivesToDraw,
      inPrimitivesType,
      draw_arrays,
      inBeginPrimitiveIndex);
}

ShaderProgram& Renderer::GetOverrideShaderProgramOr(ShaderProgram& ioAlternativeShaderProgram)
{
  const auto override_shader_program = GetOverrideShaderProgram();
  auto& shader_program = (override_shader_program ? *override_shader_program : ioAlternativeShaderProgram);
  return shader_program;
}

Renderer::UseShaderProgramBindGuard Renderer::UseShaderProgram(ShaderProgram& ioShaderProgram)
{
  Renderer::UseShaderProgramBindGuard use_shader_program_bind_guard;

  auto& shader_program = GetOverrideShaderProgramOr(ioShaderProgram);
  shader_program.Bind();

  return use_shader_program_bind_guard;
}
}