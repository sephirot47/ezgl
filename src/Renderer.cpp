#include "Renderer.h"
#include "Camera.h"
#include "DrawableMesh.h"
#include "FileUtils.h"
#include "GL.h"
#include "GLTypeTraits.h"
#include "Math.h"
#include "ProjectionParametersVariant.h"
#include "ShaderProgram.h"

namespace egl
{
Renderer::Renderer()
    : mSegmentsShaderProgram {
        VertexShader { GetFileContents("../res/segment.vert") },
        FragmentShader { GetFileContents("../res/segment.frag") },
      },
      mMeshShaderProgram {
          VertexShader { GetFileContents("../res/mesh.vert") },
          FragmentShader { GetFileContents("../res/mesh.frag") },
        }
{
  ResetCamera();
  ResetModelMatrix();
  ResetColor();
}

void Renderer::SetCamera(const Camera& camera) { mCamera = camera; }
void Renderer::ResetCamera() { mCamera = {}; }

void Renderer::SetModelMatrix(const Mat4f& inModelMatrix) { mModelMatrix = inModelMatrix; }
void Renderer::ResetModelMatrix() { mModelMatrix = {}; }

void Renderer::Translate(const Vec3f& inTranslation) { mModelMatrix = TranslationMat4(inTranslation) * mModelMatrix; }
void Renderer::Rotate(const Quatf& inRotation) { mModelMatrix = RotationMat4(inRotation) * mModelMatrix; }
void Renderer::Scale(const Vec3f& inScale) { mModelMatrix = ScaleMat4(inScale) * mModelMatrix; }

void Renderer::SetColor(const Color4f& inColor) { mColor = inColor; }
void Renderer::ResetColor() { mColor = One<Color4f>(); }

void Renderer::DrawMesh(const DrawableMesh& inDrawableMesh)
{
  mMeshShaderProgram.Bind();
  SetUniforms(mMeshShaderProgram);

  inDrawableMesh.Bind();
  const auto number_of_elements = inDrawableMesh.GetNumberOfFaces() * 3;
  static constexpr auto ElementIdType = GLTypeTraits<Mesh::VertexId>::GLType;
  GL::DrawElements(GL::EPrimitivesMode::TRIANGLES, number_of_elements, ElementIdType);
}

void Renderer::DrawAxes(const float inAxisLength)
{
  SetColor(Color4f { 1.0f, 0.0f, 0.0f, 1.0f });
  DrawSegment(Segment3f { Vec3f { 0.0f, 0.0f, 0.0f }, Vec3f { inAxisLength, 0.0f, 0.0f } });
  SetColor(Color4f { 0.0f, 1.0f, 0.0f, 1.0f });
  DrawSegment(Segment3f { Vec3f { 0.0f, 0.0f, 0.0f }, Vec3f { 0.0f, inAxisLength, 0.0f } });
  SetColor(Color4f { 0.0f, 0.0f, 1.0f, 1.0f });
  DrawSegment(Segment3f { Vec3f { 0.0f, 0.0f, 0.0f }, Vec3f { 0.0f, 0.0f, inAxisLength } });
}

void Renderer::SetUniforms(ShaderProgram& ioShaderProgram)
{
  ioShaderProgram.SetUniform("UColor", mColor);
  ioShaderProgram.SetUniform("UModel", mModelMatrix);
  ioShaderProgram.SetUniform("UView", mCamera.GetViewMatrix());
  ioShaderProgram.SetUniform("UProjection", mCamera.GetProjectionMatrix());
}
}