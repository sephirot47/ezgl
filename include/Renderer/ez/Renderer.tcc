#include "ez/Renderer.h"
#include "ez/VAO.h"
#include "ez/VBO.h"

namespace ez
{

template <typename T, std::size_t N>
void Renderer::DrawCircleSectionGeneric(const AngleRads inAngle, std::size_t inNumVertices)
{
  EXPECTS(inNumVertices >= 3);

  const auto GetCirclePoint = [inAngle, inNumVertices](std::size_t inIndex) {
    const auto progress = (inIndex / static_cast<T>(inNumVertices - 1));
    const auto angle = inAngle * progress;
    auto point = All<Vec<T, N>>(static_cast<T>(0));
    point[0] = std::cos(angle);
    point[1] = std::sin(angle);
    return point;
  };

  std::vector<Triangle<T, N>> circle_section_triangles;
  circle_section_triangles.reserve(inNumVertices - 1);
  for (std::size_t i = 0; i < inNumVertices - 1; ++i)
  {
    const auto center = All<Vec<T, N>>(0.0f);
    const auto circle_point = GetCirclePoint(i);
    const auto next_circle_point = GetCirclePoint(i + 1);
    circle_section_triangles.push_back(Triangle<T, N> { center, circle_point, next_circle_point });
  }

  DrawTrianglesGeneric(MakeSpan(circle_section_triangles));
}

template <typename T, std::size_t N>
void Renderer::DrawCircleSectionBoundaryGeneric(const AngleRads inAngle, std::size_t inNumVertices)
{
  EXPECTS(inNumVertices >= 3);

  const auto GetCirclePoint = [inAngle, inNumVertices](std::size_t inIndex) {
    const auto progress = (inIndex / static_cast<T>(inNumVertices - 1));
    const auto angle = inAngle * progress;
    auto point = All<Vec<T, N>>(static_cast<T>(0));
    point[0] = std::cos(angle);
    point[1] = std::sin(angle);
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
void Renderer::DrawTrianglesGeneric(const Span<Triangle<T, N>>& inTriangles)
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

template <typename T>
void Renderer::DrawAARectsGeneric(const Span<AARect<T>>& inAARects)
{
  // DrawMesh(MeshFactory::GetPlane());
  /*
  static_assert(N == 2 || N == 3);

  std::vector<Vec<T, N>> aa_rects_points;
  aa_rects_points.reserve(inAARects.GetNumberOfElements() * 6);
  for (const auto& aa_rect : inAARects)
  {
    auto aa_rect_point_n = Zero<Vec<T, N>>();

    aa_rect_point_n[0] = aa_rect.GetMin()[0];
    aa_rect_point_n[1] = aa_rect.GetMin()[1];
    aa_rects_points.push_back(aa_rect_point_n);

    aa_rect_point_n[0] = aa_rect.GetMax()[0];
    aa_rect_point_n[1] = aa_rect.GetMin()[1];
    aa_rects_points.push_back(aa_rect_point_n);

    aa_rect_point_n[0] = aa_rect.GetMin()[0];
    aa_rect_point_n[1] = aa_rect.GetMax()[1];
    aa_rects_points.push_back(aa_rect_point_n);

    aa_rect_point_n[0] = aa_rect.GetMin()[0];
    aa_rect_point_n[1] = aa_rect.GetMax()[1];
    aa_rects_points.push_back(aa_rect_point_n);

    aa_rect_point_n[0] = aa_rect.GetMax()[0];
    aa_rect_point_n[1] = aa_rect.GetMin()[1];
    aa_rects_points.push_back(aa_rect_point_n);

    aa_rect_point_n[0] = aa_rect.GetMax()[0];
    aa_rect_point_n[1] = aa_rect.GetMax()[1];
    aa_rects_points.push_back(aa_rect_point_n);
  }

  VAO vao;
  vao.AddVBO(std::make_shared<VBO>(aa_rects_points),
      MeshDrawData::PositionAttribLocation(),
      VAOVertexAttribT<Vec<T, N>>());
  constexpr auto add_normals = (N == 3);
  if constexpr (add_normals)
  {
    std::vector<Vec<T, N>> normals;
    normals.reserve(inAARects.GetNumberOfElements());
    for (const auto& aa_rect : inAARects) { normals.push_back(Forward<Vec3f>()); }

    vao.AddVBO(std::make_shared<VBO>(MakeSpan(normals)),
        MeshDrawData::NormalAttribLocation(),
        VAOVertexAttribT<Vec<T, N>>());
  }

  DrawVAOArrays(vao, aa_rects_points.size(), GL::EPrimitivesType::TRIANGLES, 0);
  */
}

template <typename T, std::size_t N>
void Renderer::DrawSegmentsGeneric(const Span<Segment<T, N>>& inSegments)
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
void Renderer::DrawPointsGeneric(const Span<Vec<T, N>>& inPoints)
{
  VAO vao;
  vao.AddVBO(std::make_shared<VBO>(inPoints), MeshDrawData::PositionAttribLocation(), VAOVertexAttribT<Vec<T, N>>());
  DrawVAOArrays(vao, inPoints.GetNumberOfElements(), GL::EPrimitivesType::POINTS);
}

template <typename T, std::size_t N>
void Renderer::DrawLineStripGeneric(const Span<Vec<T, N>>& inLinePoints)
{
  VAO vao;
  vao.AddVBO(std::make_shared<VBO>(inLinePoints),
      MeshDrawData::PositionAttribLocation(),
      VAOVertexAttribT<Vec<T, N>>());
  DrawVAOArrays(vao, inLinePoints.GetNumberOfElements(), GL::EPrimitivesType::LINE_STRIP);
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
  else if constexpr (StateId == Renderer::EStateId::BLEND_FACTORS)
  {
    GL::BlendFuncSeparate(inValue);
  }
  else if constexpr (StateId == Renderer::EStateId::BLEND_COLOR)
  {
    GL::BlendColor(inValue);
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
  else if constexpr (StateId == Renderer::EStateId::VIEWPORT)
  {
    return AARecti(Zero<Vec2i>(), Zero<Vec2i>());
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
  else if constexpr (StateId == Renderer::EStateId::BLEND_FACTORS)
  {
    GL::BlendFactors default_blend_factors;
    default_blend_factors.mSourceBlendFactorRGB = GL::EBlendFactor::SRC_ALPHA;
    default_blend_factors.mDestBlendFactorRGB = GL::EBlendFactor::ONE_MINUS_SRC_ALPHA;
    default_blend_factors.mSourceBlendFactorAlpha = GL::EBlendFactor::ONE;
    default_blend_factors.mDestBlendFactorAlpha = GL::EBlendFactor::ONE;
    return default_blend_factors;
  }
  else if constexpr (StateId == Renderer::EStateId::BLEND_COLOR)
  {
    return One<Color4f>();
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

template <auto TEStateIdToGuard>
RendererStateGuard<TEStateIdToGuard> Renderer::GetGuard()
{
  using RendererType = typename RendererStateGuard<TEStateIdToGuard>::TRenderer;
  auto renderer_cast = dynamic_cast<RendererType*>(this);
  if (renderer_cast == nullptr)
    THROW_EXCEPTION("Trying to get an incompatible guard for a Renderer. Make sure the State enum is of its type or a "
                    "parent type.");
  return RendererStateGuard<TEStateIdToGuard> { *renderer_cast };
}
}