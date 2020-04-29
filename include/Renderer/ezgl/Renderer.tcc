#include "ezgl/Renderer.h"
#include "ezgl/VAO.h"
#include "ezgl/VBO.h"

namespace egl
{
template <typename T, std::size_t N>
void Renderer::DrawSegmentGeneric(const Segment<T, N>& inSegment)
{
  DrawSegmentsGeneric(MakeSpan({ inSegment }));
}

template <typename T, std::size_t N>
void Renderer::DrawSegmentsGeneric(const Span<Segment<T, N>>& inSegments)
{
  const auto base_draw_setup = PrepareForDraw();

  std::vector<Vec<T, N>> segment_points;
  segment_points.reserve(inSegments.GetNumberOfElements() * 2);
  for (const auto& segment : inSegments)
  {
    segment_points.push_back(segment.GetFromPoint());
    segment_points.push_back(segment.GetToPoint());
  }
  const auto vbo = std::make_shared<VBO>(MakeSpan(segment_points));

  VAO vao;
  vao.AddVBO(vbo, MeshDrawData::PositionAttribLocation(), VAOVertexAttribT<Vec<T, N>>());
  const auto vao_bind_guard = vao.BindGuarded();

  GL::DrawArrays(GL::EPrimitivesType::LINES, inSegments.GetNumberOfElements() * 2);
}

template <typename T, std::size_t N>
void Renderer::DrawPointGeneric(const Vec<T, N>& inPoint)
{
  DrawPointsGeneric(MakeSpan({ inPoint }));
}

template <typename T, std::size_t N>
void Renderer::DrawPointsGeneric(const Span<Vec<T, N>>& inPoints)
{
  const auto base_draw_setup = PrepareForDraw();

  const auto vbo = std::make_shared<VBO>(inPoints);

  VAO vao;
  vao.AddVBO(vbo, MeshDrawData::PositionAttribLocation(), VAOVertexAttribT<Vec<T, N>>());
  const auto vao_bind_guard = vao.BindGuarded();

  GL::DrawArrays(GL::EPrimitivesType::POINTS, inPoints.GetNumberOfElements());
}

template <Renderer::EStateId StateId>
void Renderer::ApplyState(const State::ValueType<StateId>& inValue, State& ioState)
{
  if constexpr (StateId == Renderer::EStateId::OVERRIDE_SHADER_PROGRAM)
  {
    ioState.GetRenderer().SetOverrideShaderProgram(inValue);
  }
  else if constexpr (StateId == Renderer::EStateId::OVERRIDE_RENDER_TARGET)
  {
    ioState.GetRenderer().SetOverrideRenderTarget(inValue);
  }
  else if constexpr (StateId == Renderer::EStateId::OVERRIDE_FRAMEBUFFER)
  {
    ioState.GetRenderer().SetOverrideFramebuffer(inValue);
  }
  else if constexpr (StateId == Renderer::EStateId::VIEWPORT)
  {
    GL::Viewport(inValue.GetMin(), inValue.GetMax());
  }
  else if constexpr (StateId == Renderer::EStateId::DEPTH_FUNC)
  {
    GL::DepthFunc(inValue);
  }
  else if constexpr (StateId == Renderer::EStateId::DEPTH_WRITE_ENABLED)
  {
    GL::DepthMask(inValue);
  }
  else if constexpr (StateId == Renderer::EStateId::BLEND_ENABLED)
  {
    GL::SetEnabled(GL::EEnablable::BLEND, inValue);
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
  else if constexpr (StateId == Renderer::EStateId::OVERRIDE_RENDER_TARGET)
  {
    return nullptr;
  }
  else if constexpr (StateId == Renderer::EStateId::OVERRIDE_FRAMEBUFFER)
  {
    return nullptr;
  }
  else if constexpr (StateId == Renderer::EStateId::VIEWPORT)
  {
    return Recti(Zero<Vec2i>(), Zero<Vec2i>());
  }
  else if constexpr (StateId == Renderer::EStateId::DEPTH_FUNC)
  {
    return GL::EDepthFunc::LESS;
  }
  else if constexpr (StateId == Renderer::EStateId::DEPTH_WRITE_ENABLED)
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