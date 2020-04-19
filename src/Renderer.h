#pragma once

#include "Camera.h"
#include "Color.h"
#include "DrawableMesh.h"
#include "Material.h"
#include "Math.h"
#include "Segment.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include <cstdint>
#include <memory>
#include <optional>
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

  void ClearBackground(const Color4f& inClearColor);
  void ClearDepth();

  void SetDepthTestEnabled(const bool inDepthTestEnabled);
  void SetCullFaceEnabled(const bool inCullFaceEnabled);
  void SetBlendEnabled(const bool inBlendEnabled);

  void SetBlendFunc(const GL::EBlendFactor inBlendSourceFactor, const GL::EBlendFactor inBlendDestFactor);

  void SetPointSize(const float inPointSize);
  void SetLineWidth(const float inLineWidth);

  void SetCamera(const std::shared_ptr<Camera>& inCamera);
  std::shared_ptr<const Camera> GetCamera() const;
  std::shared_ptr<Camera> GetCamera();
  void ResetCamera();

  void SetModelMatrix(const Mat4f& inModelMatrix);
  void Translate(const Vec3f& inTranslation);
  void Rotate(const Quatf& inRotation);
  void Scale(const Vec3f& inScale);
  void Scale(const float inScale);
  void ResetModelMatrix();

  void SetMaterial(const Material& inMaterial);
  const Material& GetMaterial() const;
  Material& GetMaterial();
  void ResetMaterial();

  void SetSceneAmbientColor(const Color4f& inSceneAmbientColor);
  void SetLightColor(const Color4f& inLightColor);

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
  void DrawArrow(const Segment3f& inArrowSegment);

private:
  // Static resources
  static bool sStaticResourcesInited;
  static std::unique_ptr<ShaderProgram> sOnlyColorShaderProgram;
  static std::unique_ptr<ShaderProgram> sMeshShaderProgram;
  static std::unique_ptr<DrawableMesh> sCone;
  static std::shared_ptr<Texture2D> sWhiteTexture;

  struct State
  {
    std::shared_ptr<Camera> mCamera = std::make_shared<Camera>();
    Mat4f mModelMatrix = Identity<Mat4f>();

    Material mMaterial;

    bool mDepthEnabled = true;
    bool mCullFaceEnabled = true;

    bool mBlendEnabled = true;
    GL::EBlendFactor mBlendSourceFactor = GL::EBlendFactor::SRC_ALPHA;
    GL::EBlendFactor mBlendDestFactor = GL::EBlendFactor::ONE_MINUS_SRC_ALPHA;

    float mPointSize = 5.0f;
    float mLineWidth = 1.0f;

    Color4f mSceneAmbientColor = WithValue(White(), 0.1f);
    Color4f mLightColor = WithValue(White(), 1.0f);
  };
  std::stack<State> mStateStack;

  static const State DefaultState;
  State& GetCurrentState();
  const State& GetCurrentState() const;
  void ApplyState(const State& inStateToApply);

  void UseShaderProgram(ShaderProgram& ioShaderProgram);
};
}

#include "Renderer.tcc"