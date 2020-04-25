#include "Renderer3D.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "FileUtils.h"
#include "GL.h"
#include "GLGuard.h"
#include "GLTypeTraits.h"
#include "Geometry.h"
#include "Math.h"
#include "MeshDrawData.h"
#include "MeshFactory.h"
#include "PointLight.h"
#include "ShaderProgram.h"
#include "TextureFactory.h"
#include "UBO.h"
#include "Window.h"

namespace egl
{
bool Renderer3D::sStaticResourcesInited = false;
std::unique_ptr<ShaderProgram> Renderer3D::sOnlyColorShaderProgram;
std::unique_ptr<ShaderProgram> Renderer3D::sMeshShaderProgram;
std::unique_ptr<MeshDrawData> Renderer3D::sCone;

Renderer3D::Renderer3D()
{
  // Init static resources
  if (!sStaticResourcesInited)
  {
    sOnlyColorShaderProgram
        = std::make_unique<ShaderProgram>(VertexShader { std::filesystem::path("../res/OnlyColor.vert") },
            FragmentShader { std::filesystem::path("../res/OnlyColor.frag") });

    sMeshShaderProgram = std::make_unique<ShaderProgram>(VertexShader { std::filesystem::path("../res/Mesh.vert") },
        FragmentShader { std::filesystem::path("../res/Mesh.frag") });

    sCone = std::make_unique<MeshDrawData>(MeshFactory::GetCone(32));

    sStaticResourcesInited = true;
  }

  // Init lights
  mDirectionalLightsUBO.BufferDataEmpty(MaxNumberOfDirectionalLights * sizeof(GLSLDirectionalLight));
  mPointLightsUBO.BufferDataEmpty(MaxNumberOfPointLights * sizeof(GLSLPointLight));
}

void Renderer3D::SetCullFaceEnabled(const bool inCullFaceEnabled)
{
  mState.GetCurrent<Renderer3D::EStateId::CULL_FACE_ENABLED>() = inCullFaceEnabled;
  GL::SetEnabled(GL::Enablable::CULL_FACE, inCullFaceEnabled);
}

void Renderer3D::SetModelMatrix(const Mat4f& inModelMatrix)
{
  mState.GetCurrent<Renderer3D::EStateId::MODEL_MATRIX>() = inModelMatrix;
}
void Renderer3D::Translate(const Vec3f& inTranslation)
{
  auto& model_matrix = mState.GetCurrent<Renderer3D::EStateId::MODEL_MATRIX>();
  model_matrix = model_matrix * TranslationMat4(inTranslation);
}
void Renderer3D::Rotate(const Quatf& inRotation)
{
  auto& model_matrix = mState.GetCurrent<Renderer3D::EStateId::MODEL_MATRIX>();
  model_matrix = model_matrix * RotationMat4(inRotation);
}
void Renderer3D::Scale(const Vec3f& inScale)
{
  auto& model_matrix = mState.GetCurrent<Renderer3D::EStateId::MODEL_MATRIX>();
  model_matrix = model_matrix * ScaleMat4(inScale);
}
void Renderer3D::Scale(const float inScale) { Scale(Vec3f { inScale }); }

void Renderer3D::SetMaterial(const Material3D& inMaterial)
{
  mState.GetCurrent<Renderer3D::EStateId::MATERIAL>() = inMaterial;
}

void Renderer3D::SetSceneAmbientColor(const Color3f& inSceneAmbientColor)
{
  mState.GetCurrent<Renderer3D::EStateId::SCENE_AMBIENT_COLOR>() = inSceneAmbientColor;
}

void Renderer3D::AddDirectionalLight(const Vec3f& inDirection, const Color3f& inColor)
{
  EXPECTS(IsNormalized(inDirection));

  GLSLDirectionalLight directional_light;
  directional_light.mDirection
      = NormalizedSafe(XYZ(mState.GetCurrent<Renderer3D::EStateId::MODEL_MATRIX>() * XYZ0(inDirection)));
  directional_light.mColor = inColor;

  mState.GetCurrent<Renderer3D::EStateId::DIRECTIONAL_LIGHTS>().push_back(directional_light);
}

void Renderer3D::AddPointLight(const Vec3f& inPosition, const float inRange, const Color3f& inColor)
{
  EXPECTS(inRange > 0.0f);

  GLSLPointLight point_light;
  point_light.mPosition = XYZ(mState.GetCurrent<Renderer3D::EStateId::MODEL_MATRIX>() * XYZ1(inPosition));
  point_light.mRange = inRange;
  point_light.mColor = inColor;

  mState.GetCurrent<Renderer3D::EStateId::POINT_LIGHTS>().push_back(point_light);
}

void Renderer3D::PushState()
{
  Renderer::PushState();
  mState.PushAllTops();
}

void Renderer3D::PopState()
{
  Renderer::PopState();
  mState.PopAll();
}

void Renderer3D::ResetState()
{
  Renderer::ResetState();
  mState.PopAll();
  mState.PushAllDefaultValues();
  mState.ApplyCurrentState();
}

void Renderer3D::Begin(const Window& inWindow)
{
  Renderer::Begin(inWindow);

  SetDepthTestEnabled(true);
  SetCullFaceEnabled(true);

  // 3D Perspective Camera
  {
    const auto perspective_camera = GetPerspectiveCamera();
    assert(perspective_camera != nullptr);
    perspective_camera->SetAspectRatio(inWindow.GetFramebufferAspectRatio());
  }
}

void Renderer3D::DrawMesh(const Mesh& inMesh, const Renderer::EDrawType inDrawType)
{
  Renderer::DrawMesh(*sMeshShaderProgram, inMesh, inDrawType);
}

void Renderer3D::DrawMesh(const MeshDrawData& inMeshDrawData, const Renderer::EDrawType inDrawType)
{
  Renderer::DrawMesh(*sMeshShaderProgram, inMeshDrawData, inDrawType);
}

void Renderer3D::DrawArrow(const Segment3f& inArrowSegment)
{
  RENDERER_STATE_GUARD(this, Renderer3D::EStateId::MODEL_MATRIX);

  DrawSegment(inArrowSegment);
  Translate(inArrowSegment.GetToPoint());
  Rotate(LookInDirection(Direction(inArrowSegment)));
  Scale(Vec3f { 0.05f, 0.05f, 0.08f });
  DrawMesh(*sCone);
}

void Renderer3D::DrawAxes()
{
  RENDERER_STATE_GUARD(this, Renderer3D::EStateId::MATERIAL);
  ResetMaterial();

  GetMaterial().SetDiffuseColor(Red());
  DrawArrow(Segment3f { Zero<Vec3f>(), Right<Vec3f>() });

  GetMaterial().SetDiffuseColor(Green());
  DrawArrow(Segment3f { Zero<Vec3f>(), Up<Vec3f>() });

  GetMaterial().SetDiffuseColor(Blue());
  DrawArrow(Segment3f { Zero<Vec3f>(), Back<Vec3f>() });
}

void Renderer3D::DrawTriangle(const Triangle3f& inTriangle)
{
  Mesh triangle_mesh;
  triangle_mesh.AddVertex(inTriangle[0]);
  triangle_mesh.AddVertex(inTriangle[1]);
  triangle_mesh.AddVertex(inTriangle[2]);
  triangle_mesh.AddFace(0, 1, 2);

  const auto triangle_normal = Normal(inTriangle);
  triangle_mesh.SetCornerNormal(0, triangle_normal);
  triangle_mesh.SetCornerNormal(1, triangle_normal);
  triangle_mesh.SetCornerNormal(2, triangle_normal);

  DrawMesh(triangle_mesh);
}

// Helpers ========================================================================================

Renderer3D::UseShaderProgramBindGuard Renderer3D::UseShaderProgram(ShaderProgram& ioShaderProgram)
{
  auto use_shader_program_bind_guard = Renderer::UseShaderProgram(ioShaderProgram);
  assert(ioShaderProgram.IsBound());

  const auto& model_matrix = GetModelMatrix();
  const auto& current_camera = GetCamera();
  const auto view_matrix = current_camera->GetViewMatrix();
  const auto normal_matrix = NormalMat4(model_matrix);
  const auto projection_matrix = current_camera->GetProjectionMatrix();
  const auto projection_view_model_matrix = projection_matrix * view_matrix * model_matrix;
  const auto camera_world_position = current_camera->GetPosition();
  const auto camera_world_direction = Direction(current_camera->GetRotation());

  GetMaterial().Bind(ioShaderProgram);
  ioShaderProgram.SetUniformSafe("UModel", model_matrix);
  ioShaderProgram.SetUniformSafe("UNormal", normal_matrix);
  ioShaderProgram.SetUniformSafe("USceneAmbientColor", mState.GetCurrent<Renderer3D::EStateId::SCENE_AMBIENT_COLOR>());
  ioShaderProgram.SetUniformSafe("UProjectionViewModel", projection_view_model_matrix);

  // Lights
  if (GetMaterial().IsLightingEnabled())
  {
    // Directional lights
    ioShaderProgram.SetUniformBlockBindingSafe("UBlockDirectionalLights", 0);
    const auto& directional_lights = mState.GetCurrent<Renderer3D::EStateId::DIRECTIONAL_LIGHTS>();
    mDirectionalLightsUBO.BufferSubData(MakeSpan(directional_lights));
    mDirectionalLightsUBO.BindToBindingPoint(0);
    ioShaderProgram.SetUniformSafe("UNumberOfDirectionalLights", static_cast<int>(directional_lights.size()));

    // Point lights
    ioShaderProgram.SetUniformBlockBindingSafe("UBlockPointLights", 1);
    const auto& point_lights = mState.GetCurrent<Renderer3D::EStateId::POINT_LIGHTS>();
    mPointLightsUBO.BufferSubData(MakeSpan(point_lights));
    mPointLightsUBO.BindToBindingPoint(1);
    ioShaderProgram.SetUniformSafe("UNumberOfPointLights", static_cast<int>(point_lights.size()));
  }

  return use_shader_program_bind_guard;
}
}