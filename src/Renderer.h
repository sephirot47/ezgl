#pragma once

#include "Camera.h"
#include "Color.h"
#include "DirectionalLight.h"
#include "DrawableMesh.h"
#include "Framebuffer.h"
#include "Material.h"
#include "Math.h"
#include "PointLight.h"
#include "Segment.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "UBO.h"
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

  // Clears
  void ClearBackground(const Color4f& inClearColor);
  void ClearDepth();

  // Depth, Cull and Blend
  void SetDepthTestEnabled(const bool inDepthTestEnabled);
  void SetCullFaceEnabled(const bool inCullFaceEnabled);
  void SetBlendEnabled(const bool inBlendEnabled);
  void SetBlendFunc(const GL::EBlendFactor inBlendSourceFactor, const GL::EBlendFactor inBlendDestFactor);

  // Point and Line properties
  void SetPointSize(const float inPointSize);
  void SetLineWidth(const float inLineWidth);

  // Override ShaderProgram
  void SetOverrideShaderProgram(const std::shared_ptr<ShaderProgram>& inShaderProgram);

  // RenderTexture
  void SetRenderTexture(const std::shared_ptr<Texture2D>& inRenderTexture);

  // Camera
  void SetCamera(const std::shared_ptr<Camera>& inCamera);
  void ResetCamera();
  std::shared_ptr<const Camera> GetCamera() const;
  std::shared_ptr<Camera> GetCamera();

  // Transformation
  void SetModelMatrix(const Mat4f& inModelMatrix);
  void Translate(const Vec3f& inTranslation);
  void Rotate(const Quatf& inRotation);
  void Scale(const Vec3f& inScale);
  void Scale(const float inScale);
  void ResetModelMatrix();

  // Materials
  void SetMaterial(const Material& inMaterial);
  const Material& GetMaterial() const;
  void ResetMaterial();
  Material& GetMaterial();

  // Lighting
  void SetSceneAmbientColor(const Color3f& inSceneAmbientColor);
  void AddDirectionalLight(const Vec3f& inDirection, const Color3f& inColor);
  void AddPointLight(const Vec3f& inPosition, const float inRange, const Color3f& inColor);
  void ClearDirectionalLights();
  void ClearPointLights();

  // All state
  void PushState();
  void PopState();
  void ResetState();

  // Draw - 3D
  void DrawMesh(const DrawableMesh& inDrawableMesh, const Renderer::EDrawType inDrawType = Renderer::EDrawType::SOLID);
  void DrawVAOElements(const VAO& inVAO,
      const GL::Size inNumberOfElementsToDraw,
      const GL::EPrimitivesType inPrimitivesType = GL::EPrimitivesType::TRIANGLES);
  void DrawVAOArrays(const VAO& inVAO,
      const GL::Size inNumberOfPrimitivesToDraw,
      const GL::EPrimitivesType inPrimitivesType = GL::EPrimitivesType::TRIANGLES,
      const GL::Size inBeginPrimitiveIndex = 0);
  void DrawArrow(const Segment3f& inArrowSegment);
  void DrawAxes();
  void DrawPoint(const Vec3f& inPoint);
  void DrawPoints(const Span<Vec3f>& inPoints);
  void DrawSegment(const Segment3f& inSegment);
  void DrawSegments(const Span<Segment3f>& inSegments);

  // Draw - 2D (z = 0.0)
  void DrawPoint(const Vec2f& inPoint);
  void DrawPoints(const Span<Vec2f>& inPoints);
  void DrawSegment(const Segment2f& inSegment);
  void DrawSegments(const Span<Segment2f>& inSegments);

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
    std::shared_ptr<ShaderProgram> mOverrideShaderProgram;
    std::shared_ptr<Texture2D> mRenderTexture;

    bool mDepthEnabled = true;
    bool mCullFaceEnabled = true;

    bool mBlendEnabled = true;
    GL::EBlendFactor mBlendSourceFactor = GL::EBlendFactor::SRC_ALPHA;
    GL::EBlendFactor mBlendDestFactor = GL::EBlendFactor::ONE_MINUS_SRC_ALPHA;

    float mPointSize = 5.0f;
    float mLineWidth = 1.0f;

    Color3f mSceneAmbientColor = WithValue(White<Color3f>(), 0.1f);

    std::vector<GLSLDirectionalLight> mDirectionalLights;
    std::vector<GLSLPointLight> mPointLights;
  };
  std::stack<State> mStateStack;

  static constexpr auto MaxNumberOfDirectionalLights = 100;
  static constexpr auto MaxNumberOfPointLights = 100;
  UBO mDirectionalLightsUBO;
  UBO mPointLightsUBO;

  std::unique_ptr<Framebuffer> mRenderTextureFramebuffer;

  // Draw helpers
  void DrawVAOArraysOrElements(const VAO& inVAO,
      const GL::Size inNumberOfElementsToDraw,
      const GL::EPrimitivesType inPrimitivesType,
      const bool inDrawArrays,
      const GL::Size inBeginArraysPrimitiveIndex);

  // State
  static const State DefaultState;
  State& GetCurrentState();
  const State& GetCurrentState() const;
  void ApplyState(const State& inStateToApply);

  // Draw - Generic (3D and 2D)
  template <typename T, std::size_t N>
  void DrawSegmentGeneric(const Segment<T, N>& inSegment);
  template <typename T, std::size_t N>
  void DrawSegmentsGeneric(const Span<Segment<T, N>>& inSegments);
  template <typename T, std::size_t N>
  void DrawPointGeneric(const Vec<T, N>& inPoint);
  template <typename T, std::size_t N>
  void DrawPointsGeneric(const Span<Vec<T, N>>& inPoints);

  // Helpers
  using UseShaderProgramBindGuard = GLCompositeGuard<ShaderProgram, Material>;
  [[nodiscard]] UseShaderProgramBindGuard UseShaderProgram(ShaderProgram& ioShaderProgram);
};
}

#include "Renderer.tcc"