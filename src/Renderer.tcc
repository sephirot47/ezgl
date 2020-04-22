#include "Renderer.h"
#include "VAO.h"
#include "VBO.h"

namespace egl
{
template <typename T, std::size_t N>
void Renderer::DrawPointGeneric(const Vec<T, N>& inPoint)
{
  DrawPointsGeneric(MakeSpan(std::array { inPoint }));
}

template <typename T, std::size_t N>
void Renderer::DrawPointsGeneric(const Span<Vec<T, N>>& inPoints)
{
  constexpr auto PositionAttribLocation = 0;

  const auto use_shader_program_guard = UseShaderProgram(*sOnlyColorShaderProgram);

  const auto vbo = std::make_shared<VBO>(inPoints);

  GL_BIND_GUARD(VAO);
  VAO vao;
  vao.AddVBO(vbo, PositionAttribLocation, VAOVertexAttribT<Vec<T, N>>());
  vao.Bind();

  GL::DrawArrays(GL::EPrimitivesType::POINTS, 1);
}

template <typename T, std::size_t N>
void Renderer::DrawSegmentGeneric(const Segment<T, N>& inSegment)
{
  DrawSegmentsGeneric(MakeSpan(std::array { inSegment }));
}

template <typename T, std::size_t N>
void Renderer::DrawSegmentsGeneric(const Span<Segment<T, N>>& inSegments)
{
  constexpr auto PositionAttribLocation = 0;

  const auto use_shader_program_bind_guard = UseShaderProgram(*sOnlyColorShaderProgram);

  std::vector<Vec<T, N>> segment_points;
  segment_points.reserve(inSegments.GetNumberOfElements() * 2);
  for (const auto& segment : inSegments)
  {
    segment_points.push_back(segment.GetFromPoint());
    segment_points.push_back(segment.GetToPoint());
  }
  const auto vbo = std::make_shared<VBO>(MakeSpan(segment_points));

  GL_BIND_GUARD(VAO);
  VAO vao;
  vao.AddVBO(vbo, PositionAttribLocation, VAOVertexAttribT<Vec<T, N>>());
  vao.Bind();

  GL::DrawArrays(GL::EPrimitivesType::LINES, 2);
}

// clang-format off
/*
void Renderer::ApplyState(const State& inStateToApply)
{
  GL::SetEnabled(GL::Enablable::DEPTH_TEST, inStateToApply.mDepthEnabled);
  GL::SetEnabled(GL::Enablable::CULL_FACE, inStateToApply.mCullFaceEnabled);
  GL::SetEnabled(GL::Enablable::BLEND, inStateToApply.mBlendEnabled);
  GL::BlendFunc(inStateToApply.mBlendSourceFactor, inStateToApply.mBlendDestFactor);
  GL::PointSize(inStateToApply.mPointSize);
  GL::LineWidth(inStateToApply.mLineWidth);
  SetRenderTexture(inStateToApply.mRenderTexture);
}
*/

// State appliers
template <> inline void Renderer::ApplyState<ERendererStateId::CAMERA>(const std::shared_ptr<Camera>& ) { }
template <> inline void Renderer::ApplyState<ERendererStateId::MODEL_MATRIX>(const Mat4f& ) { }
template <> inline void Renderer::ApplyState<ERendererStateId::MATERIAL>(const Material& ) { }
template <> inline void Renderer::ApplyState<ERendererStateId::OVERRIDE_SHADER_PROGRAM>(const std::shared_ptr<ShaderProgram>& ) { }
template <> inline void Renderer::ApplyState<ERendererStateId::RENDER_TEXTURE>(const std::shared_ptr<Texture2D>& inRenderTexture) { SetRenderTexture(inRenderTexture); }
template <> inline void Renderer::ApplyState<ERendererStateId::DEPTH_ENABLED>(const bool& inDepthEnabled) { GL::SetEnabled(GL::Enablable::DEPTH_TEST, inDepthEnabled); }
template <> inline void Renderer::ApplyState<ERendererStateId::CULL_FACE_ENABLED>(const bool& inCullFaceEnabled) { GL::SetEnabled(GL::Enablable::CULL_FACE, inCullFaceEnabled); }
template <> inline void Renderer::ApplyState<ERendererStateId::BLEND_ENABLED>(const bool& inBlendEnabled) { GL::SetEnabled(GL::Enablable::BLEND, inBlendEnabled); }
template <> inline void Renderer::ApplyState<ERendererStateId::BLEND_SOURCE_FACTOR>(const GL::EBlendFactor& inBlendSourceFactor) { GL::BlendFunc(inBlendSourceFactor, GetCurrentState<ERendererStateId::BLEND_DEST_FACTOR>()); }
template <> inline void Renderer::ApplyState<ERendererStateId::BLEND_DEST_FACTOR>(const GL::EBlendFactor& inBlendDestFactor) { GL::BlendFunc(GetCurrentState<ERendererStateId::BLEND_SOURCE_FACTOR>(), inBlendDestFactor); }
template <> inline void Renderer::ApplyState<ERendererStateId::POINT_SIZE>(const float& inPointSize) { GL::PointSize(inPointSize); }
template <> inline void Renderer::ApplyState<ERendererStateId::LINE_WIDTH>(const float& inLineWidth) { GL::LineWidth(inLineWidth); }
template <> inline void Renderer::ApplyState<ERendererStateId::SCENE_AMBIENT_COLOR>(const Color3f& ) { }
template <> inline void Renderer::ApplyState<ERendererStateId::DIRECTIONAL_LIGHTS>(const std::vector<GLSLDirectionalLight>& ) { }
template <> inline void Renderer::ApplyState<ERendererStateId::POINT_LIGHTS>(const std::vector<GLSLPointLight>& ) { }

// State default values
template <> inline RendererStateStackValueType_t<ERendererStateId::CAMERA> Renderer::GetDefaultStateValue<ERendererStateId::CAMERA>() { return std::make_shared<Camera>(); }
template <> inline RendererStateStackValueType_t<ERendererStateId::MODEL_MATRIX> Renderer::GetDefaultStateValue<ERendererStateId::MODEL_MATRIX>() { return Identity<Mat4f>(); }
template <> inline RendererStateStackValueType_t<ERendererStateId::MATERIAL> Renderer::GetDefaultStateValue<ERendererStateId::MATERIAL>() { return Material{}; }
template <> inline RendererStateStackValueType_t<ERendererStateId::OVERRIDE_SHADER_PROGRAM> Renderer::GetDefaultStateValue<ERendererStateId::OVERRIDE_SHADER_PROGRAM>() { return nullptr; }
template <> inline RendererStateStackValueType_t<ERendererStateId::RENDER_TEXTURE> Renderer::GetDefaultStateValue<ERendererStateId::RENDER_TEXTURE>() { return nullptr; }
template <> inline RendererStateStackValueType_t<ERendererStateId::DEPTH_ENABLED> Renderer::GetDefaultStateValue<ERendererStateId::DEPTH_ENABLED>() { return true; }
template <> inline RendererStateStackValueType_t<ERendererStateId::CULL_FACE_ENABLED> Renderer::GetDefaultStateValue<ERendererStateId::CULL_FACE_ENABLED>() { return true; }
template <> inline RendererStateStackValueType_t<ERendererStateId::BLEND_ENABLED> Renderer::GetDefaultStateValue<ERendererStateId::BLEND_ENABLED>() { return true; }
template <> inline RendererStateStackValueType_t<ERendererStateId::BLEND_SOURCE_FACTOR> Renderer::GetDefaultStateValue<ERendererStateId::BLEND_SOURCE_FACTOR>() { return GL::EBlendFactor::SRC_ALPHA; }
template <> inline RendererStateStackValueType_t<ERendererStateId::BLEND_DEST_FACTOR> Renderer::GetDefaultStateValue<ERendererStateId::BLEND_DEST_FACTOR>() { return GL::EBlendFactor::ONE_MINUS_SRC_ALPHA; }
template <> inline RendererStateStackValueType_t<ERendererStateId::POINT_SIZE> Renderer::GetDefaultStateValue<ERendererStateId::POINT_SIZE>() { return 5.0f; }
template <> inline RendererStateStackValueType_t<ERendererStateId::LINE_WIDTH> Renderer::GetDefaultStateValue<ERendererStateId::LINE_WIDTH>() { return 1.0f; }
template <> inline RendererStateStackValueType_t<ERendererStateId::SCENE_AMBIENT_COLOR> Renderer::GetDefaultStateValue<ERendererStateId::SCENE_AMBIENT_COLOR>() { return WithValue(White<Color3f>(), 0.3f); }
template <> inline RendererStateStackValueType_t<ERendererStateId::DIRECTIONAL_LIGHTS> Renderer::GetDefaultStateValue<ERendererStateId::DIRECTIONAL_LIGHTS>() { return {}; }
template <> inline RendererStateStackValueType_t<ERendererStateId::POINT_LIGHTS> Renderer::GetDefaultStateValue<ERendererStateId::POINT_LIGHTS>() { return {}; }

// clang-format on
}