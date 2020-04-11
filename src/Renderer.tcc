#include "Renderer.h"
#include "VAO.h"
#include "VBO.h"

namespace egl
{
template <typename T, std::size_t N>
void Renderer::DrawSegment(const Segment<T, N>& inSegment)
{
  constexpr auto PositionAttribLocation = 0;

  mSegmentsShaderProgram.Bind();
  SetUniforms(mSegmentsShaderProgram);

  const std::array segment_points = { inSegment.GetFromPoint(), inSegment.GetToPoint() };
  const auto vbo = std::make_shared<VBO>(MakeSpan(segment_points));

  VAO vao;
  vao.Bind();
  vao.AddVBO(vbo, PositionAttribLocation, VAOVertexAttribT<Vec<T, N>>());

  GL::DrawArrays(GL::EPrimitivesMode::LINES, 2);
}
}