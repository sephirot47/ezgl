#include "Renderer.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "DrawableMesh.h"
#include "FileUtils.h"
#include "GL.h"
#include "GLTypeTraits.h"
#include "Math.h"
#include "MeshFactory.h"
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
  // Init state
  mStateStack.push(DefaultState);
  ApplyState(GetCurrentState());

  // Init static resources
  if (!sStaticResourcesInited)
  {
    sOnlyColorShaderProgram = std::make_unique<ShaderProgram>(VertexShader { GetFileContents("../res/OnlyColor.vert") },
        FragmentShader { GetFileContents("../res/OnlyColor.frag") });

    sMeshShaderProgram = std::make_unique<ShaderProgram>(VertexShader { GetFileContents("../res/Mesh.vert") },
        FragmentShader { GetFileContents("../res/Mesh.frag") });

    sCone = std::make_unique<DrawableMesh>(DrawableMeshFactory::GetCone(32));
    sWhiteTexture = TextureFactory::GetWhiteTexture();

    sStaticResourcesInited = true;
  }

  // Init lights
  mDirectionalLightsUBO.BufferDataEmpty(MaxNumberOfDirectionalLights * sizeof(GLSLDirectionalLight));
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
  directional_light.mDirection = inDirection;
  directional_light.mColor = inColor;

  GetCurrentState().mDirectionalLights.push_back(directional_light);
}
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
}

void Renderer::DrawMesh(const DrawableMesh& inDrawableMesh, const Renderer::EDrawType& inDrawType)
{
  UseShaderProgram(*sMeshShaderProgram);

  inDrawableMesh.Bind();

  if (inDrawType == EDrawType::WIREFRAME)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  static constexpr auto ElementIdType = GLTypeTraits<Mesh::VertexId>::GLType;
  const auto primitives_mode
      = (inDrawType == EDrawType::POINTS) ? GL::EPrimitivesMode::POINTS : GL::EPrimitivesMode::TRIANGLES;
  GL::DrawElements(primitives_mode, inDrawableMesh.GetNumberOfCorners(), ElementIdType);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

void Renderer::UseShaderProgram(ShaderProgram& ioShaderProgram)
{
  const auto& model_matrix = GetCurrentState().mModelMatrix;
  const auto view_matrix = GetCurrentState().mCamera->GetViewMatrix();
  const auto normal_matrix = NormalMat4(model_matrix);
  const auto projection_matrix = GetCurrentState().mCamera->GetProjectionMatrix();
  const auto projection_view_model_matrix = projection_matrix * view_matrix * model_matrix;
  const auto camera_world_position = GetCurrentState().mCamera->GetPosition();
  const auto camera_world_direction = Direction(GetCurrentState().mCamera->GetRotation());

  ioShaderProgram.Bind();
  GetMaterial().Bind(ioShaderProgram);
  ioShaderProgram.SetUniformSafe("UModel", model_matrix);
  ioShaderProgram.SetUniformSafe("UNormal", normal_matrix);
  ioShaderProgram.SetUniformSafe("UView", view_matrix);
  ioShaderProgram.SetUniformSafe("UCameraWorldPosition", camera_world_position);
  ioShaderProgram.SetUniformSafe("UCameraWorldDirection", camera_world_direction);
  ioShaderProgram.SetUniformSafe("USceneAmbientColor", GetCurrentState().mSceneAmbientColor);
  ioShaderProgram.SetUniformSafe("UProjection", projection_view_model_matrix);
  ioShaderProgram.SetUniformSafe("UProjectionViewModel", projection_view_model_matrix);

  // Lights
  if (GetMaterial().IsLightingEnabled())
  {
    ioShaderProgram.SetUniformBlockBindingSafe("UBlockLights", 0);

    // Directional lights
    const auto& directional_lights = GetCurrentState().mDirectionalLights;
    mDirectionalLightsUBO.BufferSubData(MakeSpan(directional_lights));
    mDirectionalLightsUBO.BindToBindingPoint(0);

    ioShaderProgram.SetUniformSafe("UNumberOfDirectionalLights", static_cast<int>(directional_lights.size()));
  }
}
}