#include "Renderer.h"
#include "VAO.h"
#include "VBO.h"

namespace egl
{
template <typename T, std::size_t N>
void Renderer::DrawPoint(const Vec<T, N>& inPoint)
{
  DrawPoints(MakeSpan(std::array { inPoint }));
}

template <typename T, std::size_t N>
void Renderer::DrawPoints(const Span<Vec<T, N>>& inPoints)
{
  constexpr auto PositionAttribLocation = 0;

  UseShaderProgram(*sOnlyColorShaderProgram);

  const auto vbo = std::make_shared<VBO>(inPoints);

  VAO vao;
  vao.Bind();
  vao.AddVBO(vbo, PositionAttribLocation, VAOVertexAttribT<Vec3f>());

  GL::DrawArrays(GL::EPrimitivesMode::POINTS, 1);
}

template <typename T, std::size_t N>
void Renderer::DrawSegment(const Segment<T, N>& inSegment)
{
  DrawSegments(MakeSpan(std::array { inSegment }));
}

template <typename T, std::size_t N>
void Renderer::DrawSegments(const Span<Segment<T, N>>& inSegments)
{
  constexpr auto PositionAttribLocation = 0;

  UseShaderProgram(*sOnlyColorShaderProgram);

  std::vector<Vec<T, N>> segment_points;
  segment_points.reserve(inSegments.GetNumberOfElements() * 2);
  for (const auto& segment : inSegments)
  {
    segment_points.push_back(segment.GetFromPoint());
    segment_points.push_back(segment.GetToPoint());
  }
  const auto vbo = std::make_shared<VBO>(MakeSpan(segment_points));

  VAO vao;
  vao.Bind();
  vao.AddVBO(vbo, PositionAttribLocation, VAOVertexAttribT<Vec<T, N>>());

  GL::DrawArrays(GL::EPrimitivesMode::LINES, 2);
}
}