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
  // PushDefaultValueToAllStateStacks<StateStacksTupleType, static_cast<ERendererStateId>(0)>(mStateStacks);
}

void Renderer::ClearBackground(const Color4f& inClearColor)
{
  GL::ClearColor(inClearColor);
  GL::ClearBuffer(GL::EBufferBitFlags::COLOR);
}

void Renderer::ClearDepth() { GL::ClearBuffer(GL::EBufferBitFlags::DEPTH); }

void Renderer::SetDepthTestEnabled(const bool inDepthTestEnabled)
{
  GetCurrentState<ERendererStateId::DEPTH_ENABLED>() = inDepthTestEnabled;
  GL::SetEnabled(GL::Enablable::DEPTH_TEST, inDepthTestEnabled);
}

void Renderer::SetCullFaceEnabled(const bool inCullFaceEnabled)
{
  GetCurrentState<ERendererStateId::CULL_FACE_ENABLED>() = inCullFaceEnabled;
  GL::SetEnabled(GL::Enablable::CULL_FACE, inCullFaceEnabled);
}

void Renderer::SetBlendEnabled(const bool inBlendEnabled)
{
  GetCurrentState<ERendererStateId::BLEND_ENABLED>() = inBlendEnabled;
  GL::SetEnabled(GL::Enablable::BLEND, inBlendEnabled);
}

void Renderer::SetBlendFunc(const GL::EBlendFactor inBlendSourceFactor, const GL::EBlendFactor inBlendDestFactor)
{
  GetCurrentState<ERendererStateId::BLEND_SOURCE_FACTOR>() = inBlendSourceFactor;
  GetCurrentState<ERendererStateId::BLEND_DEST_FACTOR>() = inBlendDestFactor;
  GL::BlendFunc(inBlendSourceFactor, inBlendDestFactor);
}

void Renderer::SetPointSize(const float inPointSize)
{
  GetCurrentState<ERendererStateId::POINT_SIZE>() = inPointSize;
  GL::PointSize(inPointSize);
}

void Renderer::SetLineWidth(const float inLineWidth)
{
  GetCurrentState<ERendererStateId::LINE_WIDTH>() = inLineWidth;
  GL::LineWidth(inLineWidth);
}

void Renderer::SetOverrideShaderProgram(const std::shared_ptr<ShaderProgram>& inShaderProgram)
{
  GetCurrentState<ERendererStateId::OVERRIDE_SHADER_PROGRAM>() = inShaderProgram;
}

void Renderer::SetRenderTexture(const std::shared_ptr<Texture2D>& inRenderTexture)
{
  mRenderTextureFramebuffer->SetAttachment(GL::EFramebufferAttachment::COLOR_ATTACHMENT0, inRenderTexture);
  GetCurrentState<ERendererStateId::RENDER_TEXTURE>() = inRenderTexture;

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
  GetCurrentState<ERendererStateId::CAMERA>() = inCamera;
}
std::shared_ptr<const Camera> Renderer::GetCamera() const { return GetCurrentState<ERendererStateId::CAMERA>(); }
std::shared_ptr<Camera> Renderer::GetCamera() { return GetCurrentState<ERendererStateId::CAMERA>(); }

void Renderer::SetModelMatrix(const Mat4f& inModelMatrix)
{
  GetCurrentState<ERendererStateId::MODEL_MATRIX>() = inModelMatrix;
}
void Renderer::Translate(const Vec3f& inTranslation)
{
  GetCurrentState<ERendererStateId::MODEL_MATRIX>()
      = GetCurrentState<ERendererStateId::MODEL_MATRIX>() * TranslationMat4(inTranslation);
}
void Renderer::Rotate(const Quatf& inRotation)
{
  GetCurrentState<ERendererStateId::MODEL_MATRIX>()
      = GetCurrentState<ERendererStateId::MODEL_MATRIX>() * RotationMat4(inRotation);
}
void Renderer::Scale(const Vec3f& inScale)
{
  GetCurrentState<ERendererStateId::MODEL_MATRIX>()
      = GetCurrentState<ERendererStateId::MODEL_MATRIX>() * ScaleMat4(inScale);
}
void Renderer::Scale(const float inScale) { Scale(Vec3f { inScale }); }

void Renderer::SetMaterial(const Material& inMaterial) { GetCurrentState<ERendererStateId::MATERIAL>() = inMaterial; }
const Material& Renderer::GetMaterial() const { return GetCurrentState<ERendererStateId::MATERIAL>(); }
Material& Renderer::GetMaterial() { return GetCurrentState<ERendererStateId::MATERIAL>(); }

void Renderer::SetSceneAmbientColor(const Color3f& inSceneAmbientColor)
{
  GetCurrentState<ERendererStateId::SCENE_AMBIENT_COLOR>() = inSceneAmbientColor;
}

void Renderer::AddDirectionalLight(const Vec3f& inDirection, const Color3f& inColor)
{
  EXPECTS(IsNormalized(inDirection));

  GLSLDirectionalLight directional_light;
  directional_light.mDirection
      = NormalizedSafe(XYZ(GetCurrentState<ERendererStateId::MODEL_MATRIX>() * XYZ0(inDirection)));
  directional_light.mColor = inColor;

  GetCurrentState<ERendererStateId::DIRECTIONAL_LIGHTS>().push_back(directional_light);
}

void Renderer::ClearDirectionalLights() { GetCurrentState<ERendererStateId::DIRECTIONAL_LIGHTS>().clear(); }

void Renderer::AddPointLight(const Vec3f& inPosition, const float inRange, const Color3f& inColor)
{
  EXPECTS(inRange > 0.0f);

  GLSLPointLight point_light;
  point_light.mPosition = XYZ(GetCurrentState<ERendererStateId::MODEL_MATRIX>() * XYZ1(inPosition));
  point_light.mRange = inRange;
  point_light.mColor = inColor;

  GetCurrentState<ERendererStateId::POINT_LIGHTS>().push_back(point_light);
}

void Renderer::ClearPointLights() { GetCurrentState<ERendererStateId::POINT_LIGHTS>().clear(); }

void Renderer::PushState()
{
  mStateStacks.Apply<PushStateFunctor>();
  // std::apply([](auto&... state_stack) { (..., [&]() { state_stack.push(state_stack.top()); }()); }, mStateStacks);
}

void Renderer::PopState()
{
  // Pop all
  // std::apply([](auto&... state_stack) { (..., state_stack.pop()); }, mStateStacks);

  // Apply new current state after pop, by applying the current state of each stack
  // ApplyAllStateStacks<decltype(mStateStacks), static_cast<ERendererStateId>(0)>(mStateStacks);
}

void Renderer::ResetState()
{
  PopState();
  PushState();
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
  ResetCurrentState<ERendererStateId::MATERIAL>();

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

  auto& current_override_shader_program = GetCurrentState<ERendererStateId::OVERRIDE_SHADER_PROGRAM>();
  auto& shader_program = (current_override_shader_program ? *current_override_shader_program : ioShaderProgram);

  const auto& model_matrix = GetCurrentState<ERendererStateId::MODEL_MATRIX>();
  const auto& current_camera = GetCurrentState<ERendererStateId::CAMERA>();
  const auto view_matrix = current_camera->GetViewMatrix();
  const auto normal_matrix = NormalMat4(model_matrix);
  const auto projection_matrix = current_camera->GetProjectionMatrix();
  const auto projection_view_model_matrix = projection_matrix * view_matrix * model_matrix;
  const auto camera_world_position = current_camera->GetPosition();
  const auto camera_world_direction = Direction(current_camera->GetRotation());

  shader_program.Bind();

  GetMaterial().Bind(shader_program);
  shader_program.SetUniformSafe("UModel", model_matrix);
  shader_program.SetUniformSafe("UNormal", normal_matrix);
  shader_program.SetUniformSafe("UView", view_matrix);
  shader_program.SetUniformSafe("UCameraWorldPosition", camera_world_position);
  shader_program.SetUniformSafe("UCameraWorldDirection", camera_world_direction);
  shader_program.SetUniformSafe("USceneAmbientColor", GetCurrentState<ERendererStateId::SCENE_AMBIENT_COLOR>());
  shader_program.SetUniformSafe("UProjection", projection_view_model_matrix);
  shader_program.SetUniformSafe("UProjectionViewModel", projection_view_model_matrix);

  // Lights
  if (GetMaterial().IsLightingEnabled())
  {
    // Directional lights
    shader_program.SetUniformBlockBindingSafe("UBlockDirectionalLights", 0);
    const auto& directional_lights = GetCurrentState<ERendererStateId::DIRECTIONAL_LIGHTS>();
    mDirectionalLightsUBO.BufferSubData(MakeSpan(directional_lights));
    mDirectionalLightsUBO.BindToBindingPoint(0);
    shader_program.SetUniformSafe("UNumberOfDirectionalLights", static_cast<int>(directional_lights.size()));

    // Point lights
    shader_program.SetUniformBlockBindingSafe("UBlockPointLights", 1);
    const auto& point_lights = GetCurrentState<ERendererStateId::POINT_LIGHTS>();
    mPointLightsUBO.BufferSubData(MakeSpan(point_lights));
    mPointLightsUBO.BindToBindingPoint(1);
    shader_program.SetUniformSafe("UNumberOfPointLights", static_cast<int>(point_lights.size()));
  }

  return use_shader_program_bind_guard;
}
}