#include "Renderer.h"
#include "Camera.h"
#include "DrawableMesh.h"
#include "FileUtils.h"
#include "GL.h"
#include "GLTypeTraits.h"
#include "Math.h"
#include "MeshFactory.h"
#include "ProjectionParametersVariant.h"
#include "ShaderProgram.h"

namespace egl
{

const Renderer::State Renderer::DefaultState;

Renderer::Renderer()
    : mUnshadedShaderProgram {
        VertexShader { GetFileContents("../res/unshaded.vert") },
        FragmentShader { GetFileContents("../res/unshaded.frag") },
      },
      mShadedShaderProgram {
          VertexShader { GetFileContents("../res/shaded.vert") },
          FragmentShader { GetFileContents("../res/shaded.frag") },
        }
{
  mStateStack.push(DefaultState);
  ApplyState(GetCurrentState());

  mCone = DrawableMeshFactory::GetCone(32);
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

void Renderer::SetCamera(const Camera& camera) { GetCurrentState().mCamera = camera; }
void Renderer::ResetCamera() { GetCurrentState().mCamera = DefaultState.mCamera; }

void Renderer::SetModelMatrix(const Mat4f& inModelMatrix) { GetCurrentState().mModelMatrix = inModelMatrix; }
void Renderer::ResetModelMatrix() { GetCurrentState().mModelMatrix = DefaultState.mModelMatrix; }

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

void Renderer::SetColor(const Color4f& inColor) { GetCurrentState().mColor = inColor; }
void Renderer::ResetColor() { GetCurrentState().mColor = DefaultState.mColor; }

void Renderer::SetLightAmbientColor(const Color4f& inLightAmbientColor)
{
  GetCurrentState().mLightAmbientColor = inLightAmbientColor;
}

void Renderer::SetLightDiffuseColor(const Color4f& inLightDiffuseColor)
{
  GetCurrentState().mLightDiffuseColor = inLightDiffuseColor;
}

void Renderer::SetLightSpecularColor(const Color4f& inLightSpecularColor)
{
  GetCurrentState().mLightSpecularColor = inLightSpecularColor;
}

void Renderer::SetLightSpecularExponent(const float inSpecularExponent)
{
  GetCurrentState().mLightSpecularExponent = inSpecularExponent;
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
  UseShaderProgram(mShadedShaderProgram);

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

  SetColor(Red());
  DrawArrow(Segment3f { Zero<Vec3f>(), Right<Vec3f>() });

  SetColor(Green());
  DrawArrow(Segment3f { Zero<Vec3f>(), Up<Vec3f>() });

  SetColor(Blue());
  DrawArrow(Segment3f { Zero<Vec3f>(), Forward<Vec3f>() });

  PopState();
}

void Renderer::DrawArrow(const Segment3f& inArrowSegment)
{
  PushState();

  DrawSegment(inArrowSegment);
  Translate(inArrowSegment.GetToPoint());
  Rotate(LookInDirection(Direction(inArrowSegment)));
  Rotate(-AngleAxis(QuarterCircleRads<float>(), Right<Vec3f>()));
  Scale(Vec3f { 0.05f, 0.15f, 0.05f });
  DrawMesh(mCone);

  PopState();
}

void Renderer::UseShaderProgram(ShaderProgram& ioShaderProgram)
{
  const auto& model_matrix = GetCurrentState().mModelMatrix;
  const auto view_matrix = GetCurrentState().mCamera.GetViewMatrix();
  const auto normal_matrix = NormalMat4(model_matrix);
  const auto projection_matrix = GetCurrentState().mCamera.GetProjectionMatrix();
  const auto projection_view_model_matrix = projection_matrix * view_matrix * model_matrix;
  const auto camera_world_position = GetCurrentState().mCamera.GetPosition();
  const auto camera_world_direction = Direction(GetCurrentState().mCamera.GetOrientation());

  ioShaderProgram.Bind();
  ioShaderProgram.SetUniformSafe("UColor", GetCurrentState().mColor);
  ioShaderProgram.SetUniformSafe("UModel", model_matrix);
  ioShaderProgram.SetUniformSafe("UNormal", normal_matrix);
  ioShaderProgram.SetUniformSafe("UView", view_matrix);
  ioShaderProgram.SetUniformSafe("UCameraWorldPosition", camera_world_position);
  ioShaderProgram.SetUniformSafe("UCameraWorldDirection", camera_world_direction);
  ioShaderProgram.SetUniformSafe("ULightAmbientColor", GetCurrentState().mLightAmbientColor);
  ioShaderProgram.SetUniformSafe("ULightDiffuseColor", GetCurrentState().mLightDiffuseColor);
  ioShaderProgram.SetUniformSafe("ULightSpecularColor", GetCurrentState().mLightSpecularColor);
  ioShaderProgram.SetUniformSafe("ULightSpecularExponent", GetCurrentState().mLightSpecularExponent);
  ioShaderProgram.SetUniformSafe("UProjection", projection_view_model_matrix);
  ioShaderProgram.SetUniformSafe("UProjectionViewModel", projection_view_model_matrix);
}
}