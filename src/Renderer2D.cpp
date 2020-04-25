#include "Renderer2D.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "FileUtils.h"
#include "GL.h"
#include "GLGuard.h"
#include "GLTypeTraits.h"
#include "Geometry.h"
#include "Math.h"
#include "Mesh.h"
#include "MeshDrawData.h"
#include "MeshFactory.h"
#include "PointLight.h"
#include "ShaderProgram.h"
#include "TextureFactory.h"
#include "UBO.h"
#include "Window.h"

namespace egl
{
bool Renderer2D::sStaticResourcesInited = false;
std::unique_ptr<ShaderProgram> Renderer2D::sShaderProgram;

Renderer2D::Renderer2D()
{
  // Init static resources
  if (!sStaticResourcesInited)
  {
    sShaderProgram = std::make_unique<ShaderProgram>(VertexShader { std::filesystem::path("../res/2D.vert") },
        FragmentShader { std::filesystem::path("../res/2D.frag") });

    sStaticResourcesInited = true;
  }
}

void Renderer2D::SetModelMatrix(const Mat3f& inModelMatrix)
{
  mState.GetCurrent<Renderer2D::EStateId::MODEL_MATRIX>() = inModelMatrix;
}
void Renderer2D::Translate(const Vec2f& inTranslation)
{
  auto& model_matrix = mState.GetCurrent<Renderer2D::EStateId::MODEL_MATRIX>();
  model_matrix = model_matrix * TranslationMat(inTranslation);
}
void Renderer2D::Rotate(const AngleRads inRotationAngle)
{
  auto& model_matrix = mState.GetCurrent<Renderer2D::EStateId::MODEL_MATRIX>();
  model_matrix = model_matrix * RotationMat(inRotationAngle);
}
void Renderer2D::Scale(const Vec2f& inScale)
{
  auto& model_matrix = mState.GetCurrent<Renderer2D::EStateId::MODEL_MATRIX>();
  model_matrix = model_matrix * ScaleMat(inScale);
}
void Renderer2D::Scale(const float inScale) { Scale(Vec2f { inScale }); }

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

void Renderer2D::ResetState()
{
  Renderer::ResetState();
  mState.PopAll();
  mState.PushAllDefaultValues();
  mState.ApplyCurrentState();
}

void Renderer2D::Begin(const Window& inWindow)
{
  Renderer::Begin(inWindow);

  SetDepthTestEnabled(false);
  GL::Disable(GL::Enablable::CULL_FACE);

  // 2D Orthographic Camera
  {
    const auto window_size = inWindow.GetSize();
    const float window_width = window_size[0];
    const float window_height = window_size[1];

    OrthographicParameters2f orthographic_params;
    orthographic_params.mMin = Vec2f { 0.0f, 0.0f };
    orthographic_params.mMax = Vec2f { window_width, window_height };

    const auto orthographic_camera = std::make_shared<OrthographicCamera2f>();
    orthographic_camera->SetOrthographicParameters(orthographic_params);
    SetCamera(orthographic_camera);
  }
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

void Renderer2D::DrawMesh(const Mesh& inMesh, const Renderer::EDrawType inDrawType)
{
  Renderer::DrawMesh(*sShaderProgram, inMesh, inDrawType);
}

void Renderer2D::DrawMesh(const MeshDrawData& inMeshDrawData, const Renderer::EDrawType inDrawType)
{
  Renderer::DrawMesh(*sShaderProgram, inMeshDrawData, inDrawType);
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

Renderer2D::UseShaderProgramBindGuard Renderer2D::UseShaderProgram(ShaderProgram& ioShaderProgram)
{
  auto use_shader_program_bind_guard = Renderer::UseShaderProgram(ioShaderProgram);
  assert(ioShaderProgram.IsBound());

  auto& shader_program = GetOverrideShaderProgramOr(ioShaderProgram);

  const auto& model_matrix = GetModelMatrix();
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
  return use_shader_program_bind_guard;
}
}