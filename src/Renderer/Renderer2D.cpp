#include "ezgl/Renderer2D.h"
#include "ezgl/Camera.h"
#include "ezgl/DirectionalLight.h"
#include "ezgl/FileUtils.h"
#include "ezgl/GL.h"
#include "ezgl/GLGuard.h"
#include "ezgl/GLTypeTraits.h"
#include "ezgl/Geometry.h"
#include "ezgl/Math.h"
#include "ezgl/Mesh.h"
#include "ezgl/MeshDrawData.h"
#include "ezgl/MeshFactory.h"
#include "ezgl/PointLight.h"
#include "ezgl/ShaderProgram.h"
#include "ezgl/ShaderProgramFactory.h"
#include "ezgl/TextureFactory.h"
#include "ezgl/UBO.h"
#include "ezgl/Window.h"

namespace egl
{
bool Renderer2D::sStaticResourcesInited = false;
std::shared_ptr<ShaderProgram> Renderer2D::sShaderProgram;

Renderer2D::Renderer2D()
{
  // Init static resources
  if (!sStaticResourcesInited)
  {
    sShaderProgram = ShaderProgramFactory::Get2DShaderProgram();
    sStaticResourcesInited = true;
  }

  PushAllDefaultStateValues();
}

void Renderer2D::SetTransformMatrix(const Mat3f& inTransformMatrix)
{
  mState.GetCurrent<Renderer2D::EStateId::TRANSFORM_MATRIX>() = inTransformMatrix;
}
void Renderer2D::Translate(const Vec2f& inTranslation)
{
  auto& model_matrix = mState.GetCurrent<Renderer2D::EStateId::TRANSFORM_MATRIX>();
  model_matrix = model_matrix * TranslationMat(inTranslation);
}
void Renderer2D::Rotate(const AngleRads inRotationAngle)
{
  auto& model_matrix = mState.GetCurrent<Renderer2D::EStateId::TRANSFORM_MATRIX>();
  model_matrix = model_matrix * RotationMat(inRotationAngle);
}
void Renderer2D::Scale(const Vec2f& inScale)
{
  auto& model_matrix = mState.GetCurrent<Renderer2D::EStateId::TRANSFORM_MATRIX>();
  model_matrix = model_matrix * ScaleMat(inScale);
}
void Renderer2D::Scale(const float inScale) { Scale(All<Vec2f>(inScale)); }

void Renderer2D::SetMaterial(const Material2D& inMaterial)
{
  mState.GetCurrent<Renderer2D::EStateId::MATERIAL>() = inMaterial;
}

void Renderer2D::PushState()
{
  Renderer::PushState();
  mState.PushAllTops();
}

void Renderer2D::PopState()
{
  Renderer::PopState();
  mState.PopAll();
}

void Renderer2D::PushAllDefaultStateValues()
{
  Renderer::PushAllDefaultStateValues();
  mState.PushAllDefaultValues();

  // Override default states
  Renderer::GetState().Top<Renderer::EStateId::DEPTH_FUNC>() = GL::EDepthFunc::ALWAYS;
  Renderer::GetState().Top<Renderer::EStateId::DEPTH_WRITE_ENABLED>() = true;
}

void Renderer2D::AdaptToWindow(const Window& inWindow)
{
  Renderer::AdaptToWindow(inWindow);

  AdaptCameraToWindow(inWindow);
}

void Renderer2D::SetCamera(const std::shared_ptr<OrthographicCamera2f>& inCamera)
{
  mState.GetCurrent<Renderer2D::EStateId::CAMERA>() = inCamera;
}

std::shared_ptr<OrthographicCamera2f> Renderer2D::GetCamera()
{
  return mState.GetCurrent<Renderer2D::EStateId::CAMERA>();
}

std::shared_ptr<const OrthographicCamera2f> Renderer2D::GetCamera() const
{
  return const_cast<Renderer2D&>(*this).GetCamera();
}

void Renderer2D::AdaptCameraToWindow(const Window& inWindow)
{
  // 2D Orthographic Camera
  const auto window_size = inWindow.GetSize();
  const float window_width = window_size[0];
  const float window_height = window_size[1];

  OrthographicParameters2f orthographic_params;
  orthographic_params.mMin = Vec2f { 0.0f, 0.0f };
  orthographic_params.mMax = Vec2f { window_width, window_height };

  GetCamera()->SetOrthographicParameters(orthographic_params);
}

void Renderer2D::DrawMesh(const Mesh& inMesh, const Renderer::EDrawType inDrawType)
{
  SetShaderProgram(sShaderProgram);
  Renderer::DrawMesh(inMesh, inDrawType);
}

void Renderer2D::DrawMesh(const MeshDrawData& inMeshDrawData, const Renderer::EDrawType inDrawType)
{
  SetShaderProgram(sShaderProgram);
  Renderer::DrawMesh(inMeshDrawData, inDrawType);
}

void Renderer2D::DrawPoint(const Vec2f& inPoint)
{
  SetShaderProgram(sShaderProgram);
  DrawPointGeneric(inPoint);
}

void Renderer2D::DrawPoints(const Span<Vec2f>& inPoints)
{
  SetShaderProgram(sShaderProgram);
  DrawPointsGeneric(inPoints);
}

void Renderer2D::DrawSegment(const Segment2f& inSegment)
{
  SetShaderProgram(sShaderProgram);
  DrawSegmentGeneric(inSegment);
}

void Renderer2D::DrawCircleSection(const AngleRads inAngle, const std::size_t inNumVertices)
{
  EXPECTS(inNumVertices >= 3);

  SetShaderProgram(sShaderProgram);

  std::vector<Vec2f> circle_points;
  circle_points.reserve(inNumVertices + 1);
  for (std::size_t i = 0; i < inNumVertices; ++i)
  {
    const auto progress = (i / static_cast<float>(inNumVertices - 1));
    const auto angle = inAngle * progress;
    const auto offset = Vec2f(std::cos(angle), std::sin(angle));
    const auto circle_point = offset;
    circle_points.push_back(circle_point);
  }

  DrawLineStripGeneric(MakeSpan(circle_points));
}

void Renderer2D::DrawCircle(const Circlef& inCircle, const std::size_t inNumVertices)
{
  RendererStateGuard<Renderer2D::EStateId::TRANSFORM_MATRIX> transform_guard(*this);
  Translate(inCircle.GetCenter());
  Scale(inCircle.GetRadius());
  DrawCircleSection(FullCircleRads(), inNumVertices);
}

void Renderer2D::DrawSegments(const Span<Segment2f>& inSegments)
{
  SetShaderProgram(sShaderProgram);
  DrawSegmentsGeneric(inSegments);
}

void Renderer2D::DrawLineStrip(const Span<Vec2f>& inLinePoints)
{
  SetShaderProgram(sShaderProgram);
  DrawLineStripGeneric(inLinePoints);
}

void Renderer2D::DrawTriangle(const Triangle2f& inTriangle)
{
  Mesh triangle_mesh;
  triangle_mesh.AddVertex(XY0(inTriangle[0]));
  triangle_mesh.AddVertex(XY0(inTriangle[1]));
  triangle_mesh.AddVertex(XY0(inTriangle[2]));
  triangle_mesh.AddFace(0, 1, 2);

  triangle_mesh.SetCornerNormal(0, Forward<Vec3f>());
  triangle_mesh.SetCornerNormal(1, Forward<Vec3f>());
  triangle_mesh.SetCornerNormal(2, Forward<Vec3f>());

  DrawMesh(triangle_mesh);
}

void Renderer2D::DrawTriangleBoundary(const Triangle2f& inTriangle)
{
  DrawSegments(MakeSpan({ Segment2f { inTriangle[0], inTriangle[1] },
      Segment2f { inTriangle[1], inTriangle[2] },
      Segment2f { inTriangle[2], inTriangle[0] } }));
}

// Helpers ========================================================================================

void Renderer2D::PrepareForDraw(DrawSetup& ioDrawSetup)
{
  Renderer::PrepareForDraw(ioDrawSetup);

  const auto& draw_setup_2d = static_cast<const DrawSetup2D&>(ioDrawSetup);
  assert(draw_setup_2d.mShaderProgram);
  auto& shader_program = *draw_setup_2d.mShaderProgram;
  assert(shader_program.IsBound());

  mState.ApplyCurrentState();

  GL::Disable(GL::EEnablable::CULL_FACE); // Guarded in DrawSetup2D

  const auto& model_matrix = GetTransformMatrix();
  const auto& current_camera = GetCamera();
  const auto view_matrix = current_camera->GetViewMatrix();
  const auto normal_matrix = NormalMat(model_matrix);
  const auto projection_matrix = current_camera->GetProjectionMatrix();
  const auto projection_view_model_matrix = projection_matrix * view_matrix * model_matrix;

  GetMaterial().Bind(shader_program);

  shader_program.SetUniformSafe("UModel", model_matrix);
  shader_program.SetUniformSafe("UNormal", normal_matrix);
  shader_program.SetUniformSafe("UView", view_matrix);
  shader_program.SetUniformSafe("UProjection", projection_matrix);
  shader_program.SetUniformSafe("UProjectionViewModel", projection_view_model_matrix);
}
}