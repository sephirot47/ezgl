#include "ez/Renderer.h"
#include "ez/VAO.h"
#include "ez/VBO.h"

namespace ez
{

template <typename T, std::size_t N>
void RendererGPU::DrawCircleSectionGeneric(const AngleRads inAngle, std::size_t inNumVertices)
{
  EXPECTS(inNumVertices >= 3);

  const auto GetCirclePoint = [inAngle, inNumVertices](std::size_t inIndex) {
    const auto progress = (inIndex / static_cast<T>(inNumVertices - 1));
    const auto angle = inAngle * progress;
    const auto point = WithPart<0, 1>(Zero<Vec<T, N>>(), Vec2f { std::cos(angle), std::sin(angle) });
    return point;
  };

  std::vector<Triangle<T, N>> circle_section_triangles;
  circle_section_triangles.reserve(inNumVertices - 1);
  for (std::size_t i = 0; i < inNumVertices - 1; ++i)
  {
    constexpr auto center = Zero<Vec<T, N>>();
    const auto circle_point = GetCirclePoint(i);
    const auto next_circle_point = GetCirclePoint(i + 1);
    circle_section_triangles.push_back(Triangle<T, N> { center, circle_point, next_circle_point });
  }

  DrawTrianglesGeneric(MakeSpan(circle_section_triangles));
}

template <typename T, std::size_t N>
void RendererGPU::DrawCircleSectionBoundaryGeneric(const AngleRads inAngle, std::size_t inNumVertices)
{
  EXPECTS(inNumVertices >= 3);

  const auto GetCirclePoint = [inAngle, inNumVertices](std::size_t inIndex) {
    const auto progress = (inIndex / static_cast<T>(inNumVertices - 1));
    const auto angle = inAngle * progress;
    const auto point = WithPart<0, 1>(Zero<Vec<T, N>>(), Vec2f { std::cos(angle), std::sin(angle) });
    return point;
  };

  std::vector<Vec<T, N>> circle_section_points;
  circle_section_points.reserve(inNumVertices);
  for (std::size_t i = 0; i < inNumVertices; ++i)
  {
    const auto circle_point = GetCirclePoint(i);
    circle_section_points.push_back(circle_point);
  }

  DrawLineStripGeneric(MakeSpan(circle_section_points));
}

template <typename T, std::size_t N>
void RendererGPU::DrawTrianglesGeneric(const Span<Triangle<T, N>>& inTriangles)
{
  VAO vao;
  vao.AddVBO(std::make_shared<VBO>(inTriangles), MeshDrawData::PositionAttribLocation(), VAOVertexAttribT<Vec<T, N>>());
  constexpr auto add_normals = (N == 3);
  if constexpr (add_normals)
  {
    std::vector<Vec<T, N>> normals;
    normals.reserve(inTriangles.GetNumberOfElements());
    for (const auto& triangle : inTriangles) { normals.push_back(Normal(triangle)); }

    vao.AddVBO(std::make_shared<VBO>(MakeSpan(normals)),
        MeshDrawData::NormalAttribLocation(),
        VAOVertexAttribT<Vec<T, N>>());
  }

  DrawVAOArrays(vao, inTriangles.GetNumberOfElements() * 3, GL::EPrimitivesType::TRIANGLES, 0);
}

template <typename T, std::size_t N>
void RendererGPU::DrawSegmentsGeneric(const Span<Segment<T, N>>& inSegments)
{
  std::vector<Vec<T, N>> segment_points;
  segment_points.reserve(inSegments.GetNumberOfElements() * 2);
  for (const auto& segment : inSegments)
  {
    segment_points.push_back(segment.GetFromPoint());
    segment_points.push_back(segment.GetToPoint());
  }

  VAO vao;
  vao.AddVBO(std::make_shared<VBO>(MakeSpan(segment_points)),
      MeshDrawData::PositionAttribLocation(),
      VAOVertexAttribT<Vec<T, N>>());
  DrawVAOArrays(vao, segment_points.size(), GL::EPrimitivesType::LINES);
}

template <typename T, std::size_t N>
void RendererGPU::DrawPointsGeneric(const Span<Vec<T, N>>& inPoints)
{
  VAO vao;
  vao.AddVBO(std::make_shared<VBO>(inPoints), MeshDrawData::PositionAttribLocation(), VAOVertexAttribT<Vec<T, N>>());
  DrawVAOArrays(vao, inPoints.GetNumberOfElements(), GL::EPrimitivesType::POINTS);
}

template <typename T, std::size_t N>
void RendererGPU::DrawLineStripGeneric(const Span<Vec<T, N>>& inLinePoints)
{
  VAO vao;
  vao.AddVBO(std::make_shared<VBO>(inLinePoints),
      MeshDrawData::PositionAttribLocation(),
      VAOVertexAttribT<Vec<T, N>>());
  DrawVAOArrays(vao, inLinePoints.GetNumberOfElements(), GL::EPrimitivesType::LINE_STRIP);
}

template <RendererGPU::EStateId StateId>
void RendererGPU::ApplyState(const State::ValueType<StateId>& inValue, State& ioState)
{
  if constexpr (StateId == RendererGPU::EStateId::OVERRIDE_SHADER_PROGRAM)
  {
    ioState.GetRenderer().SetOverrideShaderProgram(inValue);
  }
  else if constexpr (StateId == RendererGPU::EStateId::OVERRIDE_RENDER_TARGET)
  {
    ioState.GetRenderer().SetOverrideRenderTarget(inValue);
  }
  else if constexpr (StateId == RendererGPU::EStateId::VIEWPORT)
  {
    GL::Viewport(inValue.GetMin(), inValue.GetMax());
  }
  else if constexpr (StateId == RendererGPU::EStateId::DEPTH_FUNC)
  {
    GL::DepthFunc(inValue);
  }
  else if constexpr (StateId == RendererGPU::EStateId::DEPTH_WRITE_ENABLED)
  {
    GL::DepthMask(inValue);
  }
  else if constexpr (StateId == RendererGPU::EStateId::BLEND_ENABLED)
  {
    GL::SetEnabled(GL::EEnablable::BLEND, inValue);
  }
  else if constexpr (StateId == RendererGPU::EStateId::BLEND_FACTORS)
  {
    GL::BlendFuncSeparate(inValue);
  }
  else if constexpr (StateId == RendererGPU::EStateId::BLEND_COLOR)
  {
    GL::BlendColor(inValue);
  }
  else if constexpr (StateId == RendererGPU::EStateId::POINT_SIZE)
  {
    GL::PointSize(inValue);
  }
  else if constexpr (StateId == RendererGPU::EStateId::LINE_WIDTH)
  {
    GL::LineWidth(inValue);
  }
  else
  {
    static_assert((static_cast<int>(StateId) == -1), "ApplyState not implemented for this state id.");
  }
}

template <RendererGPU::EStateId StateId>
typename RendererGPU::State::template ValueType<StateId> RendererGPU::GetDefaultValue()
{
  if constexpr (StateId == RendererGPU::EStateId::OVERRIDE_SHADER_PROGRAM)
  {
    return nullptr;
  }
  else if constexpr (StateId == RendererGPU::EStateId::OVERRIDE_RENDER_TARGET)
  {
    return nullptr;
  }
  else if constexpr (StateId == RendererGPU::EStateId::VIEWPORT)
  {
    return AARecti(Zero<Vec2i>(), Zero<Vec2i>());
  }
  else if constexpr (StateId == RendererGPU::EStateId::DEPTH_FUNC)
  {
    return GL::EDepthFunc::LESS;
  }
  else if constexpr (StateId == RendererGPU::EStateId::DEPTH_WRITE_ENABLED)
  {
    return true;
  }
  else if constexpr (StateId == RendererGPU::EStateId::BLEND_ENABLED)
  {
    return true;
  }
  else if constexpr (StateId == RendererGPU::EStateId::BLEND_FACTORS)
  {
    GL::BlendFactors default_blend_factors;
    default_blend_factors.mSourceBlendFactorRGB = GL::EBlendFactor::SRC_ALPHA;
    default_blend_factors.mDestBlendFactorRGB = GL::EBlendFactor::ONE_MINUS_SRC_ALPHA;
    default_blend_factors.mSourceBlendFactorAlpha = GL::EBlendFactor::ONE;
    default_blend_factors.mDestBlendFactorAlpha = GL::EBlendFactor::ONE;
    return default_blend_factors;
  }
  else if constexpr (StateId == RendererGPU::EStateId::BLEND_COLOR)
  {
    return One<Color4f>();
  }
  else if constexpr (StateId == RendererGPU::EStateId::POINT_SIZE)
  {
    return 5.0f;
  }
  else if constexpr (StateId == RendererGPU::EStateId::LINE_WIDTH)
  {
    return 1.0f;
  }
  else
  {
    static_assert((static_cast<int>(StateId) == -1), "GetDefaultValue not implemented for this state id.");
  }
  return {};
}

template <auto TEStateIdToGuard>
RendererStateGuard<TEStateIdToGuard> RendererGPU::GetGuard()
{
  using RendererType = typename RendererStateGuard<TEStateIdToGuard>::TRenderer;
  auto renderer_cast = dynamic_cast<RendererType*>(this);
  if (renderer_cast == nullptr)
    THROW_EXCEPTION("Trying to get an incompatible guard for a Renderer. Make sure the State enum is of its type or a "
                    "parent type.");
  return RendererStateGuard<TEStateIdToGuard> { *renderer_cast };
}
}