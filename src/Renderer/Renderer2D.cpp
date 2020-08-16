#include "ez/Renderer2D.h"
#include "ez/Camera.h"
#include "ez/DirectionalLight.h"
#include "ez/FileUtils.h"
#include "ez/Font.h"
#include "ez/GL.h"
#include "ez/GLGuard.h"
#include "ez/GLTypeTraits.h"
#include "ez/Math.h"
#include "ez/Mesh.h"
#include "ez/MeshDrawData.h"
#include "ez/MeshFactory.h"
#include "ez/PointLight.h"
#include "ez/ShaderProgram.h"
#include "ez/ShaderProgramFactory.h"
#include "ez/TextureFactory.h"
#include "ez/UBO.h"
#include "ez/Window.h"

namespace ez
{
bool Renderer2D::sStaticResourcesInited = false;
std::shared_ptr<ShaderProgram> Renderer2D::sShaderProgram;
std::shared_ptr<ShaderProgram> Renderer2D::sTextShaderProgram;

Renderer2D::Renderer2D()
{
  // Init static resources
  if (!sStaticResourcesInited)
  {
    sShaderProgram = ShaderProgramFactory::Get2DShaderProgram();
    sTextShaderProgram = ShaderProgramFactory::Get2DTextShaderProgram();
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
void Renderer2D::Rotate(const AngleRads<float> inRotationAngle)
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
  RendererGPU::PushState();
  mState.PushAllTops();
}

void Renderer2D::PopState()
{
  RendererGPU::PopState();
  mState.PopAll();
}

void Renderer2D::PushAllDefaultStateValues()
{
  RendererGPU::PushAllDefaultStateValues();
  mState.PushAllDefaultValues();

  // Override default states
  RendererGPU::GetState().Top<RendererGPU::EStateId::DEPTH_FUNC>() = GL::EDepthFunc::ALWAYS;
  RendererGPU::GetState().Top<RendererGPU::EStateId::DEPTH_WRITE_ENABLED>() = true;
}

void Renderer2D::AdaptToWindow(const Window& inWindow)
{
  RendererGPU::AdaptToWindow(inWindow);

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

void Renderer2D::DrawMesh(const Mesh& inMesh, const RendererGPU::EDrawType inDrawType)
{
  SetShaderProgram(sShaderProgram);
  RendererGPU::DrawMesh(inMesh, inDrawType);
}

void Renderer2D::DrawMesh(const MeshDrawData& inMeshDrawData, const RendererGPU::EDrawType inDrawType)
{
  SetShaderProgram(sShaderProgram);
  RendererGPU::DrawMesh(inMeshDrawData, inDrawType);
}

void Renderer2D::DrawPoint(const Vec2f& inPoint)
{
  SetShaderProgram(sShaderProgram);
  DrawPointsGeneric(MakeSpan({ inPoint }));
}

void Renderer2D::DrawPoints(const Span<Vec2f>& inPoints)
{
  SetShaderProgram(sShaderProgram);
  DrawPointsGeneric(inPoints);
}

void Renderer2D::DrawLine(const Line2f& inLine, const float inLength)
{
  DrawSegment(Segment2f { inLine.GetOrigin(), inLine.GetPoint(inLength) });
}

void Renderer2D::DrawLines(const Span<Line2f>& inLines, const float inLength)
{
  std::vector<Segment2f> lines_segments;
  for (const auto& in_line : inLines)
    lines_segments.push_back(Segment2f { in_line.GetOrigin(), in_line.GetPoint(inLength) });
  DrawSegments(MakeSpan(lines_segments));
}

void Renderer2D::DrawRay(const Ray2f& inRay, const float inLength)
{
  DrawSegment(Segment2f { inRay.GetOrigin(), inRay.GetPoint(inLength) });
}

void Renderer2D::DrawRays(const Span<Ray2f>& inRays, const float inLength)
{
  std::vector<Segment2f> rays_segments;
  for (const auto& in_ray : inRays)
    rays_segments.push_back(Segment2f { in_ray.GetOrigin(), in_ray.GetPoint(inLength) });
  DrawSegments(MakeSpan(rays_segments));
}

void Renderer2D::DrawSegment(const Segment2f& inSegment)
{
  SetShaderProgram(sShaderProgram);
  DrawSegmentsGeneric(MakeSpan({ inSegment }));
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

void Renderer2D::DrawCircle(const Circlef& inCircle, std::size_t inNumVertices)
{
  if (IsVeryEqual(inCircle.GetRadius(), 0.0f))
    return;

  RendererStateGuard<Renderer2D::EStateId::TRANSFORM_MATRIX> transform_guard(*this);
  Translate(Center(inCircle));
  Scale(inCircle.GetRadius());
  DrawCircle(inNumVertices);
}

void Renderer2D::DrawCircleBoundary(const Circlef& inCircle, const std::size_t inNumVertices)
{
  if (IsVeryEqual(inCircle.GetRadius(), 0.0f))
    return;

  RendererStateGuard<Renderer2D::EStateId::TRANSFORM_MATRIX> transform_guard(*this);
  Translate(Center(inCircle));
  Scale(inCircle.GetRadius());
  DrawCircleBoundary(inNumVertices);
}

void Renderer2D::DrawCircleSection(const AngleRads<float> inAngle, const std::size_t inNumVertices)
{
  DrawCircleSectionGeneric<float, 2>(inAngle, inNumVertices);
}

void Renderer2D::DrawCircleSectionBoundary(const AngleRads<float> inAngle, const std::size_t inNumVertices)
{
  DrawCircleSectionBoundaryGeneric<float, 2>(inAngle, inNumVertices);
}

void Renderer2D::DrawCircle(const std::size_t inNumVertices) { DrawCircleSection(FullCircleRads(), inNumVertices); }

void Renderer2D::DrawCircleBoundary(const std::size_t inNumVertices)
{
  DrawCircleSectionBoundary(FullCircleRads(), inNumVertices);
}

void Renderer2D::DrawCapsule(const Capsule2f& inCapsule, const std::size_t inNumHemicircleSegments)
{
  if (IsVeryEqual(SqLength(inCapsule), 0.0f) || IsVeryEqual(inCapsule.GetRadius(), 0.0f))
    return;

  if (IsVeryEqual(SqLength(inCapsule), 0.0f))
  {
    DrawCircle(Circlef { inCapsule.GetOrigin(), inCapsule.GetRadius() }, inNumHemicircleSegments);
  }
  else
  {
    DrawCircle(Circlef { inCapsule.GetOrigin(), inCapsule.GetRadius() }, inNumHemicircleSegments);
    DrawCircle(Circlef { inCapsule.GetDestiny(), inCapsule.GetRadius() }, inNumHemicircleSegments);

    RendererStateGuard<Renderer2D::EStateId::TRANSFORM_MATRIX> transform_guard(*this);
    Translate(Center(inCapsule));
    Rotate(Orientation(inCapsule));
    DrawAARect(MakeAAHyperBoxFromCenterSize(Zero<Vec2f>(), Vec2f { Length(inCapsule), inCapsule.GetRadius() * 2.0f }));
  }
}

void Renderer2D::DrawTriangle(const Triangle2f& inTriangle) { DrawTriangles(MakeSpan({ inTriangle })); }

void Renderer2D::DrawTriangles(const Span<Triangle2f>& inTriangles)
{
  SetShaderProgram(sShaderProgram);
  DrawTrianglesGeneric(inTriangles);
}

void Renderer2D::DrawTriangleBoundary(const Triangle2f& inTriangle)
{
  DrawSegments(MakeSpan({ Segment2f { inTriangle[0], inTriangle[1] },
      Segment2f { inTriangle[1], inTriangle[2] },
      Segment2f { inTriangle[2], inTriangle[0] } }));
}

void Renderer2D::DrawAASquare() { DrawAARect(); }

void Renderer2D::DrawAASquare(const AASquaref& inAASquare) { DrawAARect(MakeAAHyperBoxFromAAHyperCube(inAASquare)); }

void Renderer2D::DrawAASquareBoundary() { DrawAARectBoundary(); }

void Renderer2D::DrawAASquareBoundary(const AASquaref& inAASquare)
{
  DrawAARectBoundary(MakeAAHyperBoxFromAAHyperCube(inAASquare));
}

void Renderer2D::DrawAARect() { DrawMesh(MeshFactory::GetPlane()); }

void Renderer2D::DrawAARect(const AARectf& inAARect)
{
  RendererStateGuard<Renderer2D::EStateId::TRANSFORM_MATRIX> transform_guard(*this);
  Translate(Center(inAARect));
  Scale(inAARect.GetSize());
  DrawMesh(MeshFactory::GetPlane());
}

void Renderer2D::DrawAARectBoundary()
{
  DrawSegments(MakeSpan({ Segment2f { Vec2f { -1.0f, -1.0f }, Vec2f { -1.0f, 1.0f } },
      Segment2f { Vec2f { -1.0f, 1.0f }, Vec2f { 1.0f, 1.0f } },
      Segment2f { Vec2f { 1.0f, 1.0f }, Vec2f { 1.0f, -1.0f } },
      Segment2f { Vec2f { 1.0f, -1.0f }, Vec2f { -1.0f, -1.0f } } }));
}

void Renderer2D::DrawAARectBoundary(const AARectf& inAARect)
{
  RendererStateGuard<Renderer2D::EStateId::TRANSFORM_MATRIX> transform_guard(*this);
  Translate(Center(inAARect));
  Scale(inAARect.GetSize() * 0.5f);
  DrawAARectBoundary();
}

void Renderer2D::DrawText(const std::string_view inText,
    const Font& inFont,
    const float inScale,
    const ETextHAlignment& inHAlignment,
    const ETextVAlignment& inVAlignment)
{
  RendererStateGuard<Renderer2D::EStateId::MATERIAL> material_guard(*this);
  const auto& font_atlas_texture = inFont.GetAtlasTexture();
  GetMaterial().SetTexture(font_atlas_texture);

  SetShaderProgram(sTextShaderProgram);

  RendererStateGuard<Renderer2D::EStateId::TRANSFORM_MATRIX> transform_guard(*this);
  Scale(inScale);

  const auto text_mesh = inFont.GetTextMesh(inText, inHAlignment, inVAlignment);
  RendererGPU::DrawMesh(text_mesh);
}

// Helpers ========================================================================================

void Renderer2D::PrepareForDraw(DrawSetup& ioDrawSetup)
{
  RendererGPU::PrepareForDraw(ioDrawSetup);

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