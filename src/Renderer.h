#pragma once

#include "Camera.h"
#include "Color.h"
#include "Math.h"
#include "Segment.h"
#include "ShaderProgram.h"
#include <cstdint>

namespace egl
{
class DrawableMesh;
class ShaderProgram;

class Renderer
{
public:
  Renderer();
  Renderer(const Renderer& inRHS) = default;
  Renderer& operator=(const Renderer& inRHS) = default;
  Renderer(Renderer&&) = default;
  Renderer& operator=(Renderer&& inRHS) = default;
  ~Renderer() = default;

  void SetCamera(const Camera& camera);
  void ResetCamera();

  void SetModelMatrix(const Mat4f& inModelMatrix);
  void ResetModelMatrix();

  void Translate(const Vec3f& inTranslation);
  void Rotate(const Quatf& inRotation);
  void Scale(const Vec3f& inScale);

  void SetColor(const Color4f& inColor);
  void ResetColor();

  template <typename T, std::size_t N>
  void DrawSegment(const Segment<T, N>& inSegment);
  void DrawMesh(const DrawableMesh& inDrawableMesh);
  void DrawAxes(const float inAxisLength);

private:
  Camera mCamera;
  Mat4f mModelMatrix;
  Color4f mColor;

  ShaderProgram mSegmentsShaderProgram;
  ShaderProgram mMeshShaderProgram;

  void SetUniforms(ShaderProgram& ioShaderProgram);
};
}

#include "Renderer.tcc"