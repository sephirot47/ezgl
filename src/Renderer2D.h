#pragma once
/*

#include "Camera.h"
#include "Color.h"
#include "DirectionalLight.h"
#include "Framebuffer.h"
#include "Macros.h"
#include "Material.h"
#include "Math.h"
#include "MeshDrawData.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "Plane.h"
#include "PointLight.h"
#include "RendererStateStacks.h"
#include "Segment.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Triangle.h"
#include "UBO.h"
#include "Window.h"
#include <any>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <stack>
#include <tuple>

namespace egl
{
class MeshDrawData;
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

  // Depth enabled
  void SetDepthTestEnabled(const bool inDepthTestEnabled);
  bool GetDepthTestEnabled() const { return mState.GetCurrent<ERendererStateId::DEPTH_ENABLED>(); }
  void PushDepthTestEnabled() { mState.PushTop<ERendererStateId::DEPTH_ENABLED>(); }
  void PopDepthTestEnabled() { mState.Pop<ERendererStateId::DEPTH_ENABLED>(); }
  void ResetDepthEnabled() { mState.Reset<ERendererStateId::DEPTH_ENABLED>(); }

  // Cull face enabled
  void SetCullFaceEnabled(const bool inCullFaceEnabled);
  bool GetCullFaceEnabled() const { return mState.GetCurrent<ERendererStateId::CULL_FACE_ENABLED>(); }
  void PushCullFaceEnabled() { mState.PushTop<ERendererStateId::CULL_FACE_ENABLED>(); }
  void PopCullFaceEnabled() { mState.Pop<ERendererStateId::CULL_FACE_ENABLED>(); }
  void ResetCullFaceEnabled() { mState.Reset<ERendererStateId::CULL_FACE_ENABLED>(); }

  // Blend enabled
  void SetBlendEnabled(const bool inBlendEnabled);
  bool GetBlendEnabled() const { return mState.GetCurrent<ERendererStateId::BLEND_ENABLED>(); }
  void PushBlendEnabled() { mState.PushTop<ERendererStateId::BLEND_ENABLED>(); }
  void PopBlendEnabled() { mState.Pop<ERendererStateId::BLEND_ENABLED>(); }
  void ResetBlendEnabled() { mState.Reset<ERendererStateId::BLEND_ENABLED>(); }

  // Blend func/factors
  void SetBlendFunc(const GL::EBlendFactor inBlendSourceFactor, const GL::EBlendFactor inBlendDestFactor);
  GL::EBlendFactor GetBlendSourceFactor() const { return mState.GetCurrent<ERendererStateId::BLEND_SOURCE_FACTOR>(); }
  void PushBlendSourceFactor() { mState.PushTop<ERendererStateId::BLEND_SOURCE_FACTOR>(); }
  void PopBlendSourceFactor() { mState.Pop<ERendererStateId::BLEND_SOURCE_FACTOR>(); }
  void ResetBlendSourceactor() { mState.Reset<ERendererStateId::BLEND_SOURCE_FACTOR>(); }
  GL::EBlendFactor GetBlendDestFactor() const { return mState.GetCurrent<ERendererStateId::BLEND_DEST_FACTOR>(); }
  void PushBlendDestFactor() { mState.PushTop<ERendererStateId::BLEND_DEST_FACTOR>(); }
  void PopBlendDestFactor() { mState.Pop<ERendererStateId::BLEND_DEST_FACTOR>(); }
  void ResetBlendDestFactor() { mState.Reset<ERendererStateId::BLEND_DEST_FACTOR>(); }

  // Point and Line properties
  void SetPointSize(const float inPointSize);
  float GetPointSize() const { return mState.GetCurrent<ERendererStateId::POINT_SIZE>(); }
  void PushPointSize() { mState.PushTop<ERendererStateId::POINT_SIZE>(); }
  void PopPointSize() { mState.Pop<ERendererStateId::POINT_SIZE>(); }
  void ResetPointSize() { mState.Reset<ERendererStateId::POINT_SIZE>(); }

  void SetLineWidth(const float inLineWidth);
  float GetLineWidth() const { return mState.GetCurrent<ERendererStateId::LINE_WIDTH>(); }
  void PushLineWidth() { mState.PushTop<ERendererStateId::LINE_WIDTH>(); }
  void PopLineWidth() { mState.Pop<ERendererStateId::LINE_WIDTH>(); }
  void ResetLineWidth() { mState.Reset<ERendererStateId::LINE_WIDTH>(); }

  // Override ShaderProgram
  void SetOverrideShaderProgram(const std::shared_ptr<ShaderProgram>& inShaderProgram);
  std::shared_ptr<const ShaderProgram> GetOverrideShaderProgram() const
  {
    return mState.GetCurrent<ERendererStateId::OVERRIDE_SHADER_PROGRAM>();
  }
  std::shared_ptr<ShaderProgram> GetOverrideShaderProgram()
  {
    return mState.GetCurrent<ERendererStateId::OVERRIDE_SHADER_PROGRAM>();
  }
  void PushOverrideShaderProgram() { mState.PushTop<ERendererStateId::OVERRIDE_SHADER_PROGRAM>(); }
  void PopOverrideShaderProgram() { mState.Pop<ERendererStateId::OVERRIDE_SHADER_PROGRAM>(); }
  void ResetOverrideShaderProgram() { mState.Reset<ERendererStateId::OVERRIDE_SHADER_PROGRAM>(); }

  // RenderTexture
  void SetRenderTexture(const std::shared_ptr<Texture2D>& inRenderTexture);
  std::shared_ptr<const Texture2D> GetRenderTexture() const
  {
    return mState.GetCurrent<ERendererStateId::RENDER_TEXTURE>();
  }
  std::shared_ptr<Texture2D> GetRenderTexture() { return mState.GetCurrent<ERendererStateId::RENDER_TEXTURE>(); }
  void PushRenderTexture() { mState.PushTop<ERendererStateId::RENDER_TEXTURE>(); }
  void PopRenderTexture() { mState.Pop<ERendererStateId::RENDER_TEXTURE>(); }
  void ResetRenderTexture() { mState.Reset<ERendererStateId::RENDER_TEXTURE>(); }

  // Camera
  void SetCamera(const std::shared_ptr<Camera>& inCamera);
  std::shared_ptr<Camera> GetCamera();
  std::shared_ptr<const Camera> GetCamera() const;
  std::shared_ptr<PerspectiveCamera> GetPerspectiveCamera();               // Null if it is not a PerspectiveCamera
  std::shared_ptr<const PerspectiveCamera> GetPerspectiveCamera() const;   // Null if it is not a PerspectiveCamera
  std::shared_ptr<OrthographicCamera> GetOrthographicCamera();             // Null if it is not an OrthographicCamera
  std::shared_ptr<const OrthographicCamera> GetOrthographicCamera() const; // Null if it is not an OrthographicCamera
  void PushCamera() { mState.PushTop<ERendererStateId::CAMERA>(); }
  void PopCamera() { mState.Pop<ERendererStateId::CAMERA>(); }
  void ResetCamera() { mState.Reset<ERendererStateId::CAMERA>(); }

  // Transformation
  void SetModelMatrix(const Mat4f& inModelMatrix);
  const Mat4f& GetModelMatrix() const { return mState.GetCurrent<ERendererStateId::MODEL_MATRIX>(); }
  Mat4f& GetModelMatrix() { return mState.GetCurrent<ERendererStateId::MODEL_MATRIX>(); }
  void Translate(const Vec3f& inTranslation);
  void Rotate(const Quatf& inRotation);
  void Scale(const Vec3f& inScale);
  void Scale(const float inScale);
  void PushModelMatrix() { mState.PushTop<ERendererStateId::MODEL_MATRIX>(); }
  void PopModelMatrix() { mState.Pop<ERendererStateId::MODEL_MATRIX>(); }
  void ResetModelMatrix() { mState.Reset<ERendererStateId::MODEL_MATRIX>(); }

  // Materials
  void SetMaterial(const Material& inMaterial);
  const Material& GetMaterial() const { return mState.GetCurrent<ERendererStateId::MATERIAL>(); }
  Material& GetMaterial() { return mState.GetCurrent<ERendererStateId::MATERIAL>(); }
  void PushMaterial() { mState.PushTop<ERendererStateId::MATERIAL>(); }
  void PopMaterial() { mState.Pop<ERendererStateId::MATERIAL>(); }
  void ResetMaterial() { mState.Reset<ERendererStateId::MATERIAL>(); }

  // Lighting
  void SetSceneAmbientColor(const Color3f& inSceneAmbientColor);
  const Color3f& GetSceneAmbientColor() const { return mState.GetCurrent<ERendererStateId::SCENE_AMBIENT_COLOR>(); }
  void PushSceneAmbientColor() { mState.PushTop<ERendererStateId::SCENE_AMBIENT_COLOR>(); }
  void PopSceneAmbientColor() { mState.Pop<ERendererStateId::SCENE_AMBIENT_COLOR>(); }
  void ResetSceneAmbientColor() { mState.Reset<ERendererStateId::SCENE_AMBIENT_COLOR>(); }

  void AddDirectionalLight(const Vec3f& inDirection, const Color3f& inColor);
  const auto& GetDirectionalLights() { return mState.GetCurrent<ERendererStateId::DIRECTIONAL_LIGHTS>(); }
  void PushDirectionalLights() { mState.PushTop<ERendererStateId::DIRECTIONAL_LIGHTS>(); }
  void PopDirectionalLights() { mState.Pop<ERendererStateId::DIRECTIONAL_LIGHTS>(); }
  void ResetDirectionalLights() { mState.Reset<ERendererStateId::DIRECTIONAL_LIGHTS>(); }

  void AddPointLight(const Vec3f& inPosition, const float inRange, const Color3f& inColor);
  const auto& GetPointLights() { return mState.GetCurrent<ERendererStateId::POINT_LIGHTS>(); }
  void PushPointLights() { mState.PushTop<ERendererStateId::POINT_LIGHTS>(); }
  void PopPointLights() { mState.Pop<ERendererStateId::POINT_LIGHTS>(); }
  void ResetPointLights() { mState.Reset<ERendererStateId::POINT_LIGHTS>(); }

  // All state
  void PushState();
  void PopState();
  void ResetState();

  // State generics
  template <ERendererStateId TStateId>
  auto& GetCurrent();
  template <ERendererStateId TStateId>
  const auto& GetCurrent() const;
  template <ERendererStateId TStateId>
  void Push();
  template <ERendererStateId TStateId>
  void Pop();

  // Draw - 3D
  void PrepareFor3D(const Window& inWindow);
  void DrawMesh(const Mesh& inMesh, const Renderer::EDrawType inDrawType = Renderer::EDrawType::SOLID);
  void DrawMesh(const MeshDrawData& inMeshDrawData, const Renderer::EDrawType inDrawType = Renderer::EDrawType::SOLID);
  void DrawVAOElements(const VAO& inVAO,
      const GL::Size inNumberOfElementsToDraw,
      const GL::EPrimitivesType inPrimitivesType = GL::EPrimitivesType::TRIANGLES);
  void DrawVAOArrays(const VAO& inVAO,
      const GL::Size inNumberOfPrimitivesToDraw,
      const GL::EPrimitivesType inPrimitivesType = GL::EPrimitivesType::TRIANGLES,
      const GL::Size inBeginPrimitiveIndex = 0);
  void DrawArrow(const Segment3f& inArrowSegment);
  void DrawAxes();
  void DrawPoint(const Vec3f& inPoint) { DrawPointGeneric(inPoint); }
  void DrawPoints(const Span<Vec3f>& inPoints) { DrawPointsGeneric(inPoints); }
  void DrawSegment(const Segment3f& inSegment) { DrawSegmentGeneric(inSegment); }
  void DrawSegments(const Span<Segment3f>& inSegments) { DrawSegmentsGeneric(inSegments); }
  void DrawTriangle(const Triangle3f& inTriangle);

  // Draw - 2D (z = 0.0)
  void PrepareFor2D(const Window& inWindow);
  void DrawPoint(const Vec2f& inPoint) { DrawPointGeneric(inPoint); }
  void DrawPoints(const Span<Vec2f>& inPoints) { DrawPointsGeneric(inPoints); }
  void DrawSegment(const Segment2f& inSegment) { DrawSegmentGeneric(inSegment); }
  void DrawSegments(const Span<Segment2f>& inSegments) { DrawSegmentsGeneric(inSegments); }
  void DrawTriangle(const Triangle2f& inTriangle);
  void DrawTriangleBoundary(const Triangle2f& inTriangle);

private:
  // Static resources
  static bool sStaticResourcesInited;
  static std::unique_ptr<ShaderProgram> sOnlyColorShaderProgram;
  static std::unique_ptr<ShaderProgram> sMeshShaderProgram;
  static std::unique_ptr<MeshDrawData> sCone;

  // State
  RendererStateStacks<Renderer> mState { *this };

  // Lights
  static constexpr auto MaxNumberOfDirectionalLights = 100;
  static constexpr auto MaxNumberOfPointLights = 100;
  UBO mDirectionalLightsUBO;
  UBO mPointLightsUBO;

  // Draw helpers
  void DrawVAOArraysOrElements(const VAO& inVAO,
      const GL::Size inNumberOfElementsToDraw,
      const GL::EPrimitivesType inPrimitivesType,
      const bool inDrawArrays,
      const GL::Size inBeginArraysPrimitiveIndex);

  // Helpers or common functionality
  void PrepareFor3DOr2DCommon(const Window& inWindow);
  using UseShaderProgramBindGuard = GLCompositeGuard<ShaderProgram, Material>;
  [[nodiscard]] UseShaderProgramBindGuard UseShaderProgram(ShaderProgram& ioShaderProgram);
};

// Renderer state guards
template <ERendererStateId TStateId>
class RendererStateGuard final
{
public:
  RendererStateGuard(Renderer& ioRenderer) : mRenderer(ioRenderer) { mRenderer.Push<TStateId>(); }
  ~RendererStateGuard() { mRenderer.Pop<TStateId>(); }

private:
  Renderer& mRenderer;
};
}

#include "Renderer.tcc"
};
}*/