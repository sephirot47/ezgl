#include "Renderer.h"
#include "VAO.h"
#include "VBO.h"

namespace egl
{
template <typename T, std::size_t N>
void Renderer::DrawSegmentGeneric(const Segment<T, N>& inSegment, ShaderProgram& ioShaderProgram)
{
  DrawSegmentsGeneric(MakeSpan({ inSegment }), ioShaderProgram);
}

template <typename T, std::size_t N>
void Renderer::DrawSegmentsGeneric(const Span<Segment<T, N>>& inSegments, ShaderProgram& ioShaderProgram)
{
  const auto use_shader_program_bind_guard = UseShaderProgram(ioShaderProgram);

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
  vao.AddVBO(vbo, MeshDrawData::PositionAttribLocation(), VAOVertexAttribT<Vec<T, N>>());
  vao.Bind();

  GL::DrawArrays(GL::EPrimitivesType::LINES, inSegments.GetNumberOfElements() * 2);
}

template <typename T, std::size_t N>
void Renderer::DrawPointGeneric(const Vec<T, N>& inPoint, ShaderProgram& ioShaderProgram)
{
  DrawPointsGeneric(MakeSpan({ inPoint }), ioShaderProgram);
}

template <typename T, std::size_t N>
void Renderer::DrawPointsGeneric(const Span<Vec<T, N>>& inPoints, ShaderProgram& ioShaderProgram)
{
  const auto use_shader_program_guard = UseShaderProgram(ioShaderProgram);

  const auto vbo = std::make_shared<VBO>(inPoints);

  GL_BIND_GUARD(VAO);
  VAO vao;
  vao.AddVBO(vbo, MeshDrawData::PositionAttribLocation(), VAOVertexAttribT<Vec<T, N>>());
  vao.Bind();

  GL::DrawArrays(GL::EPrimitivesType::POINTS, inPoints.GetNumberOfElements());
}

template <Renderer::EStateId StateId>
void Renderer::ApplyState(const State::ValueType<StateId>& inValue, State& ioState)
{
  if constexpr (StateId == Renderer::EStateId::OVERRIDE_SHADER_PROGRAM)
  {
  }
  else if constexpr (StateId == Renderer::EStateId::RENDER_TEXTURE)
  {
    ioState.GetRenderer().SetRenderTexture(inValue);
  }
  else if constexpr (StateId == Renderer::EStateId::DEPTH_ENABLED)
  {
    GL::SetEnabled(GL::Enablable::DEPTH_TEST, inValue);
  }
  else if constexpr (StateId == Renderer::EStateId::BLEND_ENABLED)
  {
    GL::SetEnabled(GL::Enablable::BLEND, inValue);
  }
  else if constexpr (StateId == Renderer::EStateId::BLEND_SOURCE_FACTOR)
  {
    GL::BlendFunc(inValue, ioState.GetCurrent<Renderer::EStateId::BLEND_DEST_FACTOR>());
  }
  else if constexpr (StateId == Renderer::EStateId::BLEND_DEST_FACTOR)
  {
    GL::BlendFunc(ioState.GetCurrent<Renderer::EStateId::BLEND_SOURCE_FACTOR>(), inValue);
  }
  else if constexpr (StateId == Renderer::EStateId::POINT_SIZE)
  {
    GL::PointSize(inValue);
  }
  else if constexpr (StateId == Renderer::EStateId::LINE_WIDTH)
  {
    GL::LineWidth(inValue);
  }
  else
  {
    static_assert((static_cast<int>(StateId) == -1), "ApplyState not implemented for this state id.");
  }
}

template <Renderer::EStateId StateId>
typename Renderer::State::template ValueType<StateId> Renderer::GetDefaultValue()
{
  if constexpr (StateId == Renderer::EStateId::OVERRIDE_SHADER_PROGRAM)
  {
    return nullptr;
  }
  else if constexpr (StateId == Renderer::EStateId::RENDER_TEXTURE)
  {
    return nullptr;
  }
  else if constexpr (StateId == Renderer::EStateId::DEPTH_ENABLED)
  {
    return true;
  }
  else if constexpr (StateId == Renderer::EStateId::BLEND_ENABLED)
  {
    return true;
  }
  else if constexpr (StateId == Renderer::EStateId::BLEND_SOURCE_FACTOR)
  {
    return GL::EBlendFactor::SRC_ALPHA;
  }
  else if constexpr (StateId == Renderer::EStateId::BLEND_DEST_FACTOR)
  {
    return GL::EBlendFactor::ONE_MINUS_SRC_ALPHA;
  }
  else if constexpr (StateId == Renderer::EStateId::POINT_SIZE)
  {
    return 5.0f;
  }
  else if constexpr (StateId == Renderer::EStateId::LINE_WIDTH)
  {
    return 1.0f;
  }
  else
  {
    static_assert((static_cast<int>(StateId) == -1), "GetDefaultValue not implemented for this state id.");
  }
  return {};
}

}