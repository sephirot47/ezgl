#pragma once

#include "Camera.h"
#include "Color.h"
#include "Math.h"
#include "Segment.h"
#include "DrawableMesh.h"
#include "ShaderProgram.h"
#include <cstdint>
#include <stack>

namespace egl
{
class DrawableMesh;
class ShaderProgram;

class Renderer
{
public:
  enum class EDrawType
  {
    SOLID,
    WIREFRAME,
    POINTS
  };

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
  void Scale(const float inScale);

  void SetColor(const Color4f& inColor);
  void ResetColor();

  void PushState();
  void PopState();
  void ResetState();

  template <typename T, std::size_t N>
  void DrawSegment(const Segment<T, N>& inSegment);
  template <typename T, std::size_t N>
  void DrawSegments(const Span<Segment<T, N>>& inSegments);

  template <typename T, std::size_t N>
  void DrawPoint(const Vec<T, N>& inPoint);
  template <typename T, std::size_t N>
  void DrawPoints(const Span<Vec<T, N>>& inPoints);

  void DrawMesh(const DrawableMesh& inDrawableMesh, const Renderer::EDrawType& inDrawType = Renderer::EDrawType::SOLID);
  void DrawAxes();
  void DrawArrow(const Segment3f &inArrowSegment);
  void DrawThickArrow(const Segment3f &inArrowSegment);

private:
  struct State
  {
    Camera mCamera;
    Mat4f mModelMatrix = Identity<Mat4f>();
    Color4f mColor = One<Color4f>();
  };

  std::stack<State> mStateStack;
  ShaderProgram mUnshadedShaderProgram;
  ShaderProgram mShadedShaderProgram;

  DrawableMesh mCone;

  static const State DefaultState;
  State &GetCurrentState();
  const State &GetCurrentState() const;

  void UseShaderProgram(ShaderProgram& ioShaderProgram);
};
}

#include "Renderer.tcc"