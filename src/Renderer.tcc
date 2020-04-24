#include "Renderer.h"
#include "VAO.h"
#include "VBO.h"

namespace egl
{
template <ERendererStateId TStateId>
auto& Renderer::GetCurrent()
{
  return mState.GetCurrent<TStateId>();
}

template <ERendererStateId TStateId>
const auto& Renderer::GetCurrent() const
{
  return const_cast<Renderer&>(*this).GetCurrent<TStateId>();
}

template <ERendererStateId TStateId>
void Renderer::Push()
{
  mState.PushTop<TStateId>();
}

template <ERendererStateId TStateId>
void Renderer::Pop()
{
  mState.Pop<TStateId>();
}

template <typename T, std::size_t N>
void Renderer::DrawPointGeneric(const Vec<T, N>& inPoint)
{
  DrawPointsGeneric(MakeSpan({ inPoint }));
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

  GL::DrawArrays(GL::EPrimitivesType::POINTS, inPoints.GetNumberOfElements());
}

template <typename T, std::size_t N>
void Renderer::DrawSegmentGeneric(const Segment<T, N>& inSegment)
{
  DrawSegmentsGeneric(MakeSpan({ inSegment }));
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

  GL::DrawArrays(GL::EPrimitivesType::LINES, inSegments.GetNumberOfElements() * 2);
}
}