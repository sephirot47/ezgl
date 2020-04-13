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

  mCone = DrawableMeshFactory::GetCone(32);
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

void Renderer::PushState() { mStateStack.push(GetCurrentState()); }

void Renderer::PopState()
{
  EXPECTS(mStateStack.size() >= 2);
  mStateStack.pop();
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

void Renderer::DrawMesh(const DrawableMesh& inDrawableMesh, const Renderer::EDrawType& inDrawType)
{
  UseShaderProgram(mShadedShaderProgram);

  inDrawableMesh.Bind();

  if (inDrawType == EDrawType::WIREFRAME)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  const auto number_of_elements = inDrawableMesh.GetNumberOfFaces() * 3;
  static constexpr auto ElementIdType = GLTypeTraits<Mesh::VertexId>::GLType;
  const auto primitives_mode
      = (inDrawType == EDrawType::POINTS) ? GL::EPrimitivesMode::POINTS : GL::EPrimitivesMode::TRIANGLES;
  GL::DrawElements(primitives_mode, number_of_elements, ElementIdType);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::DrawAxes()
{
  PushState();

  SetColor(Color4f { 1.0f, 0.0f, 0.0f, 1.0f });
  DrawArrow(Segment3f { Zero<Vec3f>(), Right<Vec3f>() });

  SetColor(Color4f { 0.0f, 1.0f, 0.0f, 1.0f });
  DrawArrow(Segment3f { Zero<Vec3f>(), Up<Vec3f>() });

  SetColor(Color4f { 0.0f, 0.0f, 1.0f, 1.0f });
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

void Renderer::DrawThickArrow(const Segment3f& inArrowSegment) {}

void Renderer::UseShaderProgram(ShaderProgram& ioShaderProgram)
{
  const auto &model_matrix = GetCurrentState().mModelMatrix;
  const auto view_matrix = GetCurrentState().mCamera.GetViewMatrix();
  const auto view_model_matrix = view_matrix * model_matrix;
  const auto normal_matrix = NormalMat4(view_model_matrix);
  const auto projection_matrix = GetCurrentState().mCamera.GetProjectionMatrix();
  const auto projection_view_model_matrix = projection_matrix * view_model_matrix;
  const auto camera_world_direction = Direction(GetCurrentState().mCamera.GetOrientation());

  ioShaderProgram.Bind();
  ioShaderProgram.SetUniformSafe("UColor", GetCurrentState().mColor);
  ioShaderProgram.SetUniformSafe("UModel", model_matrix);
  ioShaderProgram.SetUniformSafe("UNormal", normal_matrix);
  ioShaderProgram.SetUniformSafe("UView", view_matrix);
  ioShaderProgram.SetUniformSafe("UCameraWorldDirection", camera_world_direction);
  ioShaderProgram.SetUniformSafe("UProjection", projection_view_model_matrix);
  ioShaderProgram.SetUniformSafe("UProjectionViewModel", projection_view_model_matrix);
}
}