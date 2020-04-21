#include "Renderer.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "DrawableMesh.h"
#include "FileUtils.h"
#include "GL.h"
#include "GLGuard.h"
#include "GLTypeTraits.h"
#include "Math.h"
#include "MeshFactory.h"
#include "PointLight.h"
#include "ProjectionParametersVariant.h"
#include "ShaderProgram.h"
#include "TextureFactory.h"
#include "UBO.h"

namespace egl
{

const Renderer::State Renderer::DefaultState;
bool Renderer::sStaticResourcesInited = false;
std::unique_ptr<ShaderProgram> Renderer::sOnlyColorShaderProgram;
std::unique_ptr<ShaderProgram> Renderer::sMeshShaderProgram;
std::unique_ptr<DrawableMesh> Renderer::sCone;
std::shared_ptr<Texture2D> Renderer::sWhiteTexture;

Renderer::Renderer()
{
  // Init static resources
  if (!sStaticResourcesInited)
  {
    sOnlyColorShaderProgram
        = std::make_unique<ShaderProgram>(VertexShader { std::filesystem::path("../res/OnlyColor.vert") },
            FragmentShader { std::filesystem::path("../res/OnlyColor.frag") });

    sMeshShaderProgram = std::make_unique<ShaderProgram>(VertexShader { std::filesystem::path("../res/Mesh.vert") },
        FragmentShader { std::filesystem::path("../res/Mesh.frag") });

    sCone = std::make_unique<DrawableMesh>(DrawableMeshFactory::GetCone(32));
    sWhiteTexture = TextureFactory::GetWhiteTexture();

    sStaticResourcesInited = true;
  }

  // Init lights
  mDirectionalLightsUBO.BufferDataEmpty(MaxNumberOfDirectionalLights * sizeof(GLSLDirectionalLight));
  mPointLightsUBO.BufferDataEmpty(MaxNumberOfPointLights * sizeof(GLSLPointLight));

  // Init render texture
  mRenderTextureFramebuffer = std::make_unique<Framebuffer>();
  mRenderTextureFramebuffer->CreateRenderbuffer(GL::EFramebufferAttachment::DEPTH_STENCIL_ATTACHMENT,
      GL::ETextureInternalFormat::DEPTH24_STENCIL8);

  // Init state
  mStateStack.push(DefaultState);
  ApplyState(GetCurrentState());
}

void Renderer::ClearBackground(const Color4f& inClearColor)
{
  GL::ClearColor(inClearColor);
  GL::ClearBuffer(GL::EBufferBitFlags::COLOR);
}

void Renderer::ClearDepth() { GL::ClearBuffer(GL::EBufferBitFlags::DEPTH); }

void Renderer::SetDepthTestEnabled(const bool inDepthTestEnabled)
{
  GetCurrentState().mDepthEnabled = inDepthTestEnabled;
  GL::SetEnabled(GL::Enablable::DEPTH_TEST, inDepthTestEnabled);
}

void Renderer::SetCullFaceEnabled(const bool inCullFaceEnabled)
{
  GetCurrentState().mCullFaceEnabled = inCullFaceEnabled;
  GL::SetEnabled(GL::Enablable::CULL_FACE, inCullFaceEnabled);
}

void Renderer::SetBlendEnabled(const bool inBlendEnabled)
{
  GetCurrentState().mBlendEnabled = inBlendEnabled;
  GL::SetEnabled(GL::Enablable::BLEND, inBlendEnabled);
}

void Renderer::SetBlendFunc(const GL::EBlendFactor inBlendSourceFactor, const GL::EBlendFactor inBlendDestFactor)
{
  GetCurrentState().mBlendSourceFactor = inBlendSourceFactor;
  GetCurrentState().mBlendDestFactor = inBlendDestFactor;
  GL::BlendFunc(inBlendSourceFactor, inBlendDestFactor);
}

void Renderer::SetPointSize(const float inPointSize)
{
  GetCurrentState().mPointSize = inPointSize;
  GL::PointSize(inPointSize);
}

void Renderer::SetLineWidth(const float inLineWidth)
{
  GetCurrentState().mLineWidth = inLineWidth;
  GL::LineWidth(inLineWidth);
}

void Renderer::SetOverrideShaderProgram(const std::shared_ptr<ShaderProgram>& inShaderProgram)
{
  GetCurrentState().mOverrideShaderProgram = inShaderProgram;
}

void Renderer::SetRenderTexture(const std::shared_ptr<Texture2D>& inRenderTexture)
{
  mRenderTextureFramebuffer->SetAttachment(GL::EFramebufferAttachment::COLOR_ATTACHMENT0, inRenderTexture);
  GetCurrentState().mRenderTexture = inRenderTexture;

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

void Renderer::SetCamera(const std::shared_ptr<Camera>& inCamera) { GetCurrentState().mCamera = inCamera; }
std::shared_ptr<const Camera> Renderer::GetCamera() const { return GetCurrentState().mCamera; }
std::shared_ptr<Camera> Renderer::GetCamera() { return GetCurrentState().mCamera; }
void Renderer::ResetCamera() { GetCurrentState().mCamera = DefaultState.mCamera; }

void Renderer::SetModelMatrix(const Mat4f& inModelMatrix) { GetCurrentState().mModelMatrix = inModelMatrix; }
void Renderer::Translate(const Vec3f& inTranslation)
{
  GetCurrentState().mModelMatrix = GetCurrentState().mModelMatrix * TranslationMat4(inTranslation);
}
void Renderer::Rotate(const Quatf& inRotation)
{
  GetCurrentState().mModelMatrix = GetCurrentState().mModelMatrix * RotationMat4(inRotation);
}
void Renderer::Scale(const Vec3f& inScale)
{
  GetCurrentState().mModelMatrix = GetCurrentState().mModelMatrix * ScaleMat4(inScale);
}
void Renderer::Scale(const float inScale) { Scale(Vec3f { inScale }); }
void Renderer::ResetModelMatrix() { GetCurrentState().mModelMatrix = DefaultState.mModelMatrix; }

void Renderer::SetMaterial(const Material& inMaterial) { GetCurrentState().mMaterial = inMaterial; }
const Material& Renderer::GetMaterial() const { return GetCurrentState().mMaterial; }
Material& Renderer::GetMaterial() { return GetCurrentState().mMaterial; }
void Renderer::ResetMaterial() { GetCurrentState().mMaterial = Material(); }

void Renderer::SetSceneAmbientColor(const Color3f& inSceneAmbientColor)
{
  GetCurrentState().mSceneAmbientColor = inSceneAmbientColor;
}

void Renderer::AddDirectionalLight(const Vec3f& inDirection, const Color3f& inColor)
{
  EXPECTS(IsNormalized(inDirection));

  GLSLDirectionalLight directional_light;
  directional_light.mDirection = NormalizedSafe(XYZ(GetCurrentState().mModelMatrix * XYZ0(inDirection)));
  directional_light.mColor = inColor;

  GetCurrentState().mDirectionalLights.push_back(directional_light);
}

void Renderer::ClearDirectionalLights() { GetCurrentState().mDirectionalLights.clear(); }

void Renderer::AddPointLight(const Vec3f& inPosition, const float inRange, const Color3f& inColor)
{
  EXPECTS(inRange > 0.0f);

  GLSLPointLight point_light;
  point_light.mPosition = XYZ(GetCurrentState().mModelMatrix * XYZ1(inPosition));
  point_light.mRange = inRange;
  point_light.mColor = inColor;

  GetCurrentState().mPointLights.push_back(point_light);
}

void Renderer::ClearPointLights() { GetCurrentState().mPointLights.clear(); }

void Renderer::PushState() { mStateStack.push(GetCurrentState()); }

void Renderer::PopState()
{
  EXPECTS(mStateStack.size() >= 2);
  mStateStack.pop();

  ApplyState(GetCurrentState());
}

void Renderer::ResetState()
{
  EXPECTS(mStateStack.size() >= 2);
  mStateStack.top() = DefaultState;
}

Renderer::State& Renderer::GetCurrentState()
{
  EXPECTS(!mStateStack.empty());
  return mStateStack.top();
}

const Renderer::State& Renderer::GetCurrentState() const
{
  EXPECTS(!mStateStack.empty());
  return mStateStack.top();
}

void Renderer::ApplyState(const State& inStateToApply)
{
  GL::SetEnabled(GL::Enablable::DEPTH_TEST, inStateToApply.mDepthEnabled);
  GL::SetEnabled(GL::Enablable::CULL_FACE, inStateToApply.mCullFaceEnabled);
  GL::SetEnabled(GL::Enablable::BLEND, inStateToApply.mBlendEnabled);
  GL::BlendFunc(inStateToApply.mBlendSourceFactor, inStateToApply.mBlendDestFactor);
  GL::PointSize(inStateToApply.mPointSize);
  GL::LineWidth(inStateToApply.mLineWidth);
  SetRenderTexture(inStateToApply.mRenderTexture);
}

// Draw - 3D ========================================================================================

void Renderer::DrawMesh(const DrawableMesh& inDrawableMesh, const Renderer::EDrawType inDrawType)
{
  if (inDrawType == EDrawType::WIREFRAME)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  const auto primitives_type
      = (inDrawType == EDrawType::POINTS) ? GL::EPrimitivesType::POINTS : GL::EPrimitivesType::TRIANGLES;

  DrawVAOElements(inDrawableMesh.GetVAO(), inDrawableMesh.GetNumberOfCorners(), primitives_type);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // TODO: Restore this properly
}

void Renderer::DrawVAOArraysOrElements(const VAO& inVAO,
    const GL::Size inNumberOfElementsToDraw,
    const GL::EPrimitivesType inPrimitivesType,
    const bool inDrawArrays,
    const GL::Size inBeginArraysPrimitiveIndex)
{
  const auto use_shader_program_bind_guard = UseShaderProgram(*sMeshShaderProgram);

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

void Renderer::DrawArrow(const Segment3f& inArrowSegment)
{
  PushState();

  DrawSegment(inArrowSegment);
  Translate(inArrowSegment.GetToPoint());
  Rotate(LookInDirection(Direction(inArrowSegment)));
  Scale(Vec3f { 0.05f, 0.05f, 0.08f });
  DrawMesh(*sCone);

  PopState();
}

void Renderer::DrawAxes()
{
  PushState();
  ResetMaterial();

  GetMaterial().SetDiffuseColor(Red());
  DrawArrow(Segment3f { Zero<Vec3f>(), Right<Vec3f>() });

  GetMaterial().SetDiffuseColor(Green());
  DrawArrow(Segment3f { Zero<Vec3f>(), Up<Vec3f>() });

  GetMaterial().SetDiffuseColor(Blue());
  DrawArrow(Segment3f { Zero<Vec3f>(), Back<Vec3f>() });

  PopState();
}

void Renderer::DrawPoint(const Vec3f& inPoint) { DrawPointGeneric(inPoint); }
void Renderer::DrawPoints(const Span<Vec3f>& inPoints) { DrawPointsGeneric(inPoints); }
void Renderer::DrawSegment(const Segment3f& inSegment) { DrawSegmentGeneric(inSegment); }
void Renderer::DrawSegments(const Span<Segment3f>& inSegments) { DrawSegmentsGeneric(inSegments); }

// Draw - 2D ========================================================================================

void Renderer::DrawPoint(const Vec2f& inPoint) { DrawPointGeneric(inPoint); }
void Renderer::DrawPoints(const Span<Vec2f>& inPoints) { DrawPointsGeneric(inPoints); }
void Renderer::DrawSegment(const Segment2f& inSegment) { DrawSegmentGeneric(inSegment); }
void Renderer::DrawSegments(const Span<Segment2f>& inSegments) { DrawSegmentsGeneric(inSegments); }

// Helpers ========================================================================================

Renderer::UseShaderProgramBindGuard Renderer::UseShaderProgram(ShaderProgram& ioShaderProgram)
{
  Renderer::UseShaderProgramBindGuard use_shader_program_bind_guard;

  auto& current_state = GetCurrentState();
  auto& shader_program
      = (current_state.mOverrideShaderProgram ? *current_state.mOverrideShaderProgram : ioShaderProgram);

  const auto& model_matrix = current_state.mModelMatrix;
  const auto view_matrix = current_state.mCamera->GetViewMatrix();
  const auto normal_matrix = NormalMat4(model_matrix);
  const auto projection_matrix = current_state.mCamera->GetProjectionMatrix();
  const auto projection_view_model_matrix = projection_matrix * view_matrix * model_matrix;
  const auto camera_world_position = current_state.mCamera->GetPosition();
  const auto camera_world_direction = Direction(current_state.mCamera->GetRotation());

  shader_program.Bind();

  GetMaterial().Bind(shader_program);
  shader_program.SetUniformSafe("UModel", model_matrix);
  shader_program.SetUniformSafe("UNormal", normal_matrix);
  shader_program.SetUniformSafe("UView", view_matrix);
  shader_program.SetUniformSafe("UCameraWorldPosition", camera_world_position);
  shader_program.SetUniformSafe("UCameraWorldDirection", camera_world_direction);
  shader_program.SetUniformSafe("USceneAmbientColor", current_state.mSceneAmbientColor);
  shader_program.SetUniformSafe("UProjection", projection_view_model_matrix);
  shader_program.SetUniformSafe("UProjectionViewModel", projection_view_model_matrix);

  // Lights
  if (GetMaterial().IsLightingEnabled())
  {
    // Directional lights
    shader_program.SetUniformBlockBindingSafe("UBlockDirectionalLights", 0);
    const auto& directional_lights = current_state.mDirectionalLights;
    mDirectionalLightsUBO.BufferSubData(MakeSpan(directional_lights));
    mDirectionalLightsUBO.BindToBindingPoint(0);
    shader_program.SetUniformSafe("UNumberOfDirectionalLights", static_cast<int>(directional_lights.size()));

    // Point lights
    shader_program.SetUniformBlockBindingSafe("UBlockPointLights", 1);
    const auto& point_lights = current_state.mPointLights;
    mPointLightsUBO.BufferSubData(MakeSpan(point_lights));
    mPointLightsUBO.BindToBindingPoint(1);
    shader_program.SetUniformSafe("UNumberOfPointLights", static_cast<int>(point_lights.size()));
  }

  return use_shader_program_bind_guard;
}
}