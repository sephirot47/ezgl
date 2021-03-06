#include "ez/Renderer3D.h"
#include <ez/Camera.h>
#include <ez/DirectionalLight.h>
#include <ez/FileUtils.h>
#include <ez/Font.h>
#include <ez/GL.h>
#include <ez/GLGuard.h>
#include <ez/GLTypeTraits.h>
#include <ez/HyperSphere.h>
#include <ez/Math.h>
#include <ez/MeshDrawData.h>
#include <ez/MeshFactory.h>
#include <ez/PointLight.h>
#include <ez/ShaderProgram.h>
#include <ez/ShaderProgramFactory.h>
#include <ez/TextureFactory.h>
#include <ez/UBO.h>
#include <ez/Window.h>

namespace ez
{
bool Renderer3D::sStaticResourcesInited = false;
std::shared_ptr<ShaderProgram> Renderer3D::sOnlyColorShaderProgram;
std::shared_ptr<ShaderProgram> Renderer3D::sMeshShaderProgram;
std::shared_ptr<ShaderProgram> Renderer3D::sTextShaderProgram;
std::shared_ptr<MeshDrawData> Renderer3D::sCone;

Renderer3D::Renderer3D()
{
  // Init static resources
  if (!sStaticResourcesInited)
  {
    sOnlyColorShaderProgram = ShaderProgramFactory::GetOnlyColorShaderProgram();
    sMeshShaderProgram = ShaderProgramFactory::GetMeshShaderProgram();
    sTextShaderProgram = ShaderProgramFactory::GetTextShaderProgram();

    sCone = std::make_shared<MeshDrawData>(MeshFactory::GetCone(32));

    sStaticResourcesInited = true;
  }

  // Init lights
  mDirectionalLightsUBO.BufferDataEmpty(MaxNumberOfDirectionalLights * sizeof(GLSLDirectionalLight));
  mPointLightsUBO.BufferDataEmpty(MaxNumberOfPointLights * sizeof(GLSLPointLight));

  PushAllDefaultStateValues();
}

void Renderer3D::SetCullFaceEnabled(const bool inCullFaceEnabled)
{
  mState.GetCurrent<Renderer3D::EStateId::CULL_FACE_ENABLED>() = inCullFaceEnabled;
}

void Renderer3D::SetTransformMatrix(const Mat4f& inTransformMatrix)
{
  mState.GetCurrent<Renderer3D::EStateId::TRANSFORM_MATRIX>() = inTransformMatrix;
}
void Renderer3D::Translate(const Vec3f& inTranslation)
{
  auto& model_matrix = mState.GetCurrent<Renderer3D::EStateId::TRANSFORM_MATRIX>();
  model_matrix = model_matrix * TranslationMat(inTranslation);
}
void Renderer3D::Rotate(const Quatf& inRotation)
{
  auto& model_matrix = mState.GetCurrent<Renderer3D::EStateId::TRANSFORM_MATRIX>();
  model_matrix = model_matrix * RotationMat(inRotation);
}
void Renderer3D::Scale(const Vec3f& inScale)
{
  auto& model_matrix = mState.GetCurrent<Renderer3D::EStateId::TRANSFORM_MATRIX>();
  model_matrix = model_matrix * ScaleMat(inScale);
}
void Renderer3D::Scale(const float inScale) { Scale(All<Vec3f>(inScale)); }

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
      = NormalizedSafe(XYZ(mState.GetCurrent<Renderer3D::EStateId::TRANSFORM_MATRIX>() * XYZ0(inDirection)));
  directional_light.mColor = inColor;

  mState.GetCurrent<Renderer3D::EStateId::DIRECTIONAL_LIGHTS>().push_back(directional_light);
}

void Renderer3D::AddPointLight(const Vec3f& inPosition, const float inRange, const Color3f& inColor)
{
  EXPECTS(inRange > 0.0f);

  GLSLPointLight point_light;
  point_light.mPosition = XYZ(mState.GetCurrent<Renderer3D::EStateId::TRANSFORM_MATRIX>() * XYZ1(inPosition));
  point_light.mRange = inRange;
  point_light.mColor = inColor;

  mState.GetCurrent<Renderer3D::EStateId::POINT_LIGHTS>().push_back(point_light);
}

void Renderer3D::SetCamera(const std::shared_ptr<Camera3f>& inCamera)
{
  mState.GetCurrent<Renderer3D::EStateId::CAMERA>() = inCamera;
}

std::shared_ptr<Camera3f> Renderer3D::GetCamera() { return mState.GetCurrent<Renderer3D::EStateId::CAMERA>(); }
std::shared_ptr<const Camera3f> Renderer3D::GetCamera() const { return const_cast<Renderer3D&>(*this).GetCamera(); }

void Renderer3D::AdaptCameraToWindow(const Window& inWindow)
{
  const auto perspective_camera = GetPerspectiveCamera();
  if (perspective_camera)
    perspective_camera->SetAspectRatio(inWindow.GetFramebufferAspectRatio());
}

std::shared_ptr<PerspectiveCameraf> Renderer3D::GetPerspectiveCamera()
{
  return std::dynamic_pointer_cast<PerspectiveCameraf>(mState.GetCurrent<Renderer3D::EStateId::CAMERA>());
}

std::shared_ptr<const PerspectiveCameraf> Renderer3D::GetPerspectiveCamera() const
{
  return const_cast<Renderer3D&>(*this).GetPerspectiveCamera();
}

std::shared_ptr<OrthographicCamera3f> Renderer3D::GetOrthographicCamera()
{
  return std::dynamic_pointer_cast<OrthographicCamera3f>(mState.GetCurrent<Renderer3D::EStateId::CAMERA>());
}

std::shared_ptr<const OrthographicCamera3f> Renderer3D::GetOrthographicCamera() const
{
  return const_cast<Renderer3D&>(*this).GetOrthographicCamera();
}

void Renderer3D::PushState()
{
  RendererGPU::PushState();
  mState.PushAllTops();
}

void Renderer3D::PopState()
{
  RendererGPU::PopState();
  mState.PopAll();
}

void Renderer3D::PushAllDefaultStateValues()
{
  RendererGPU::PushAllDefaultStateValues();
  mState.PushAllDefaultValues();
}

void Renderer3D::AdaptToWindow(const Window& inWindow)
{
  RendererGPU::AdaptToWindow(inWindow);

  AdaptCameraToWindow(inWindow);
}

void Renderer3D::DrawCustom(const std::function<void()>& inCustomDrawFunction)
{
  RendererGPU::DrawCustom(inCustomDrawFunction);
}

void Renderer3D::DrawMesh(const Mesh& inMesh, const RendererGPU::EDrawType inDrawType)
{
  SetShaderProgram(sMeshShaderProgram);
  RendererGPU::DrawMesh(inMesh, inDrawType);
}

void Renderer3D::DrawMesh(const MeshDrawData& inMeshDrawData, const RendererGPU::EDrawType inDrawType)
{
  SetShaderProgram(sMeshShaderProgram);
  RendererGPU::DrawMesh(inMeshDrawData, inDrawType);
}

void Renderer3D::DrawVAOElements(const VAO& inVAO,
    const GL::Size inNumberOfElementsToDraw,
    const GL::EPrimitivesType inPrimitivesType)
{
  SetShaderProgram(sMeshShaderProgram);
  RendererGPU::DrawVAOElements(inVAO, inNumberOfElementsToDraw, inPrimitivesType);
}

void Renderer3D::DrawVAOArrays(const VAO& inVAO,
    const GL::Size inNumberOfPrimitivesToDraw,
    const GL::EPrimitivesType inPrimitivesType,
    const GL::Size inBeginPrimitiveIndex)
{
  SetShaderProgram(sMeshShaderProgram);
  RendererGPU::DrawVAOArrays(inVAO, inNumberOfPrimitivesToDraw, inPrimitivesType, inBeginPrimitiveIndex);
}

void Renderer3D::DrawArrow(const Segment3f& inArrowSegment)
{
  RENDERER_STATE_GUARD(this, Renderer3D::EStateId::TRANSFORM_MATRIX);

  DrawSegment(inArrowSegment);
  Translate(inArrowSegment.GetDestiny());
  Rotate(Orientation(inArrowSegment));
  Scale(Vec3f { 0.05f, 0.05f, 0.08f });
  DrawMesh(*sCone);
}

void Renderer3D::DrawAxes()
{
  RENDERER_STATE_GUARD(this, Renderer3D::EStateId::MATERIAL);
  ResetMaterial();

  GetMaterial().SetDiffuseColor(Red<Color4f>());
  DrawArrow(Segment3f { Zero<Vec3f>(), Right<Vec3f>() });

  GetMaterial().SetDiffuseColor(Green<Color4f>());
  DrawArrow(Segment3f { Zero<Vec3f>(), Up<Vec3f>() });

  GetMaterial().SetDiffuseColor(Blue<Color4f>());
  DrawArrow(Segment3f { Zero<Vec3f>(), Back<Vec3f>() });
}

void Renderer3D::DrawRay(const Ray3f& inRay, const float inRayDistance)
{
  DrawSegment(Segment3f { inRay.GetOrigin(), inRay.GetPoint(inRayDistance) });
}

void Renderer3D::DrawPlane(const Planef& inPlane, const float inPlaneSize)
{
  RendererStateGuard<Renderer3D::EStateId::TRANSFORM_MATRIX> transform_guard(*this);
  Translate(inPlane.GetArbitraryPoint());
  Rotate(FromTo(Forward<Vec3f>(), inPlane.GetNormal()));
  Scale(inPlaneSize);

  RendererStateGuard<Renderer3D::EStateId::CULL_FACE_ENABLED> cull_face_guard(*this);
  SetCullFaceEnabled(false);

  DrawAARect();
}

void Renderer3D::DrawPoint(const Vec3f& inPoint) { DrawPoints(MakeSpan({ inPoint })); }

void Renderer3D::DrawPoints(const Span<Vec3f>& inPoints)
{
  SetShaderProgram(sOnlyColorShaderProgram);
  DrawPointsGeneric(inPoints);
}

void Renderer3D::DrawSegment(const Segment3f& inSegment) { DrawSegments(MakeSpan({ inSegment })); }

void Renderer3D::DrawSegments(const Span<Segment3f>& inSegments)
{
  SetShaderProgram(sOnlyColorShaderProgram);
  DrawSegmentsGeneric(inSegments);
}

void Renderer3D::DrawLine(const Line3f &inLine)
{
  DrawSegment({inLine.GetPoint(-9999.9f), inLine.GetPoint(9999.9f)});
}

void Renderer3D::DrawLineStrip(const Span<Vec3f>& inLinePoints)
{
  SetShaderProgram(sOnlyColorShaderProgram);
  DrawLineStripGeneric(inLinePoints);
}

void Renderer3D::DrawTriangle(const Triangle3f& inTriangle) { DrawTriangles(MakeSpan({ inTriangle })); }

void Renderer3D::DrawTriangles(const Span<Triangle3f>& inTriangles)
{
  SetShaderProgram(sMeshShaderProgram);
  DrawTrianglesGeneric(inTriangles);
}

void Renderer3D::DrawTriangleBoundary(const Triangle3f& inTriangle)
{
  DrawSegments(MakeSpan({ Segment3f(inTriangle[0], inTriangle[1]),
      Segment3f(inTriangle[0], inTriangle[2]),
      Segment3f(inTriangle[1], inTriangle[2]) }));
}

void Renderer3D::DrawAABox(const AABoxf& inAABox)
{
  RendererStateGuard<Renderer3D::EStateId::TRANSFORM_MATRIX> transform_guard(*this);
  Translate(Center(inAABox));
  Scale(inAABox.GetSize() * 0.5f);
  DrawAABox();
}

void Renderer3D::DrawAABoxBoundary(const AABoxf& inAABox)
{
  RendererStateGuard<Renderer3D::EStateId::TRANSFORM_MATRIX> transform_guard(*this);
  Translate(Center(inAABox));
  Scale(inAABox.GetSize() * 0.5f);
  DrawAABoxBoundary();
}

void Renderer3D::DrawAABox() { DrawMesh(MeshFactory::GetBox()); }

void Renderer3D::DrawAABoxBoundary()
{
  DrawSegments(MakeSpan({ Segment3f { Vec3f { -1.0f, -1.0f, -1.0f }, Vec3f { -1.0f, -1.0f, 1.0f } },
      Segment3f { Vec3f { -1.0f, -1.0f, 1.0f }, Vec3f { 1.0f, -1.0f, 1.0f } },
      Segment3f { Vec3f { 1.0f, -1.0f, 1.0f }, Vec3f { 1.0f, -1.0f, -1.0f } },
      Segment3f { Vec3f { 1.0f, -1.0f, -1.0f }, Vec3f { -1.0f, -1.0f, -1.0f } },
      Segment3f { Vec3f { -1.0f, 1.0f, -1.0f }, Vec3f { -1.0f, 1.0f, 1.0f } },
      Segment3f { Vec3f { -1.0f, 1.0f, 1.0f }, Vec3f { 1.0f, 1.0f, 1.0f } },
      Segment3f { Vec3f { 1.0f, 1.0f, 1.0f }, Vec3f { 1.0f, 1.0f, -1.0f } },
      Segment3f { Vec3f { 1.0f, 1.0f, -1.0f }, Vec3f { -1.0f, 1.0f, -1.0f } },
      Segment3f { Vec3f { -1.0f, -1.0f, -1.0f }, Vec3f { -1.0f, 1.0f, -1.0f } },
      Segment3f { Vec3f { -1.0f, -1.0f, 1.0f }, Vec3f { -1.0f, 1.0f, 1.0f } },
      Segment3f { Vec3f { 1.0f, -1.0f, 1.0f }, Vec3f { 1.0f, 1.0f, 1.0f } },
      Segment3f { Vec3f { 1.0f, -1.0f, -1.0f }, Vec3f { 1.0f, 1.0f, -1.0f } } }));
}

void Renderer3D::DrawAARect() { DrawMesh(MeshFactory::GetPlane()); }

void Renderer3D::DrawAARect(const AARectf& inAARect)
{
  Translate(XY0(Center(inAARect)));
  Scale(XY1(inAARect.GetSize()) * 0.5f);
  DrawMesh(MeshFactory::GetPlane());
}

void Renderer3D::DrawAARectBoundary()
{
  DrawSegments(MakeSpan({ Segment3f { Vec3f { -1.0f, -1.0f, 0.0f }, Vec3f { -1.0f, 1.0f, 0.0f } },
      Segment3f { Vec3f { -1.0f, 1.0f, 0.0f }, Vec3f { 1.0f, 1.0f, 0.0f } },
      Segment3f { Vec3f { 1.0f, 1.0f, 0.0f }, Vec3f { 1.0f, -1.0f, 0.0f } },
      Segment3f { Vec3f { 1.0f, -1.0f, 0.0f }, Vec3f { -1.0f, -1.0f, 0.0f } } }));
}

void Renderer3D::DrawAARectBoundary(const AARectf& inAARect)
{
  Translate(XY0(Center(inAARect)));
  Scale(XY1(inAARect.GetSize()) * 0.5f);
  DrawAARectBoundary();
}

void Renderer3D::DrawBox(const Boxf& inBox)
{
  PushTransformMatrix();
  Translate(Center(inBox));
  Rotate(Orientation(inBox));
  Scale(inBox.GetExtents());
  DrawAABox();
  PopTransformMatrix();
}

void Renderer3D::DrawCapsule(const Capsule3f& inCapsule,
    const std::size_t inNumHemisphereLatitudes,
    const std::size_t inNumLongitudes)
{
  const auto transform_guard = GetGuard<Renderer3D::EStateId::TRANSFORM_MATRIX>();
  Translate(Center(inCapsule));
  Rotate(Orientation(inCapsule));
  DrawMesh(MeshFactory::GetCapsule(inCapsule.GetRadius(),
      Length(inCapsule.GetSegment()),
      inNumHemisphereLatitudes,
      inNumLongitudes));
}

void Renderer3D::DrawCylinder(const Cylinderf& inCylinder, const std::size_t inNumLongitudes)
{
  const auto cylinder_length = Length(inCylinder.GetSegment());
  const auto transform_guard = GetGuard<Renderer3D::EStateId::TRANSFORM_MATRIX>();
  Translate(Center(inCylinder));
  Rotate(Orientation(inCylinder));
  Scale(Vec3f { inCylinder.GetRadius() * 2.0f, inCylinder.GetRadius() * 2.0f, cylinder_length });
  DrawCylinder(inNumLongitudes);
}

void Renderer3D::DrawCylinder(std::size_t inNumLongitudes) { DrawMesh(MeshFactory::GetCylinder(inNumLongitudes)); }

void Renderer3D::DrawTorus(std::size_t inNumLatitudes, std::size_t inNumLongitudes, float inHoleSize)
{
  DrawMesh(MeshFactory::GetTorus(inNumLatitudes, inNumLongitudes, inHoleSize));
}

void Renderer3D::DrawCone(std::size_t inNumLongitudes) { DrawMesh(MeshFactory::GetCone(inNumLongitudes)); }

void Renderer3D::DrawHemisphere(std::size_t inNumLatitudes, std::size_t inNumLongitudes)
{
  DrawMesh(MeshFactory::GetHemisphere(inNumLatitudes, inNumLongitudes));
}

void Renderer3D::DrawSphere(std::size_t inNumLatitudes, std::size_t inNumLongitudes)
{
  DrawMesh(MeshFactory::GetSphere(inNumLatitudes, inNumLongitudes));
}

void Renderer3D::DrawSphere(const Spheref& inSphere, std::size_t inNumLatitudes, std::size_t inNumLongitudes)
{
  RendererStateGuard<Renderer3D::EStateId::TRANSFORM_MATRIX> transform_guard(*this);
  Translate(Center(inSphere));
  Scale(inSphere.GetRadius());
  DrawSphere(inNumLatitudes, inNumLongitudes);
}

void Renderer3D::DrawCircle(std::size_t inNumVertices)
{
  DrawCircleSectionGeneric<float, 3>(FullCircleRads(), inNumVertices);
}

void Renderer3D::DrawCircleBoundary(std::size_t inNumVertices)
{
  DrawCircleSectionBoundaryGeneric<float, 3>(FullCircleRads(), inNumVertices);
}

void Renderer3D::DrawCircleSection(const AngleRads<float> inAngle, std::size_t inNumVertices)
{
  DrawCircleSectionGeneric<float, 3>(inAngle, inNumVertices);
}

void Renderer3D::DrawCircleSectionBoundary(const AngleRads<float> inAngle, std::size_t inNumVertices)
{
  DrawCircleSectionBoundaryGeneric<float, 3>(inAngle, inNumVertices);
}

void Renderer3D::DrawText(const std::string_view inText,
    const Font& inFont,
    const float inScale,
    const ETextHAlignment& inHAlignment,
    const ETextVAlignment& inVAlignment,
    bool inBillboard,
    bool inConstantScale)
{
  RendererStateGuard<Renderer3D::EStateId::MATERIAL> material_guard(*this);
  const auto& font_atlas_texture = inFont.GetAtlasTexture();
  GetMaterial().SetTexture(font_atlas_texture);

  RendererStateGuard<Renderer3D::EStateId::CULL_FACE_ENABLED> cull_face_enabled_guard(*this);
  SetCullFaceEnabled(false);

  RendererStateGuard<Renderer3D::EStateId::TRANSFORM_MATRIX> transform_guard(*this);
  Scale(inScale);

  if (inBillboard || inConstantScale)
  {
    const auto translation_world = Translation(GetTransformMatrix());
    const auto translation_to_camera = (GetCamera()->GetPosition() - translation_world);
    if (inBillboard)
    {
      const auto dir_to_camera = NormalizedSafe(translation_to_camera);
      const auto billboard_rotation = LookInDirection(-dir_to_camera);
      Rotate(billboard_rotation);
    }

    if (inConstantScale)
    {
      const auto distance_to_camera = Length(translation_to_camera);
      Scale(Max(distance_to_camera, 0.00001f));
    }
  }

  SetShaderProgram(sTextShaderProgram);

  const auto text_mesh = inFont.GetTextMesh(inText, inHAlignment, inVAlignment);
  RendererGPU::DrawMesh(text_mesh);
}

// Helpers ========================================================================================

void Renderer3D::PrepareForDraw(DrawSetup& ioDrawSetup)
{
  RendererGPU::PrepareForDraw(ioDrawSetup);

  const auto& draw_setup_3d = static_cast<DrawSetup3D&>(ioDrawSetup);
  assert(draw_setup_3d.mShaderProgram);

  auto& shader_program = *draw_setup_3d.mShaderProgram;
  assert(shader_program.IsBound());

  mState.ApplyCurrentState();

  GetMaterial().Bind(shader_program);

  const auto& model_matrix = GetTransformMatrix();
  const auto& current_camera = GetCamera();
  const auto view_matrix = current_camera->GetViewMatrix();
  const auto normal_matrix = NormalMat(model_matrix);
  const auto projection_matrix = current_camera->GetProjectionMatrix();
  const auto projection_view_model_matrix = projection_matrix * view_matrix * model_matrix;

  shader_program.SetUniformSafe("UModel", model_matrix);
  shader_program.SetUniformSafe("UNormal", normal_matrix);
  shader_program.SetUniformSafe("UView", view_matrix);
  shader_program.SetUniformSafe("UProjection", projection_matrix);
  shader_program.SetUniformSafe("UProjectionViewModel", projection_view_model_matrix);

  const auto camera_world_position = current_camera->GetPosition();
  const auto camera_world_direction = Direction(current_camera->GetRotation());
  shader_program.SetUniformSafe("UCameraWorldPosition", camera_world_position);
  shader_program.SetUniformSafe("UCameraWorldDirection", camera_world_direction);

  shader_program.SetUniformSafe("USceneAmbientColor", GetSceneAmbientColor());

  // Lights
  if (GetMaterial().IsLightingEnabled())
  {
    // Directional lights
    shader_program.SetUniformBlockBindingSafe("UBlockDirectionalLights", 0);
    const auto& directional_lights = mState.GetCurrent<Renderer3D::EStateId::DIRECTIONAL_LIGHTS>();
    mDirectionalLightsUBO.BufferSubData(MakeSpan(directional_lights));
    mDirectionalLightsUBO.BindToBindingPoint(0);
    shader_program.SetUniformSafe("UNumberOfDirectionalLights", static_cast<int>(directional_lights.size()));

    // Point lights
    shader_program.SetUniformBlockBindingSafe("UBlockPointLights", 1);
    const auto& point_lights = mState.GetCurrent<Renderer3D::EStateId::POINT_LIGHTS>();
    mPointLightsUBO.BufferSubData(MakeSpan(point_lights));
    mPointLightsUBO.BindToBindingPoint(1);
    shader_program.SetUniformSafe("UNumberOfPointLights", static_cast<int>(point_lights.size()));
  }
}
}