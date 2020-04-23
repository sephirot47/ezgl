#include "PerspectiveCamera.h"
#include "RendererStateStacks.h"

namespace egl
{
template <typename TRenderer>
template <ERendererStateId TStateId>
void RendererStateStacks<TRenderer>::ApplyState(const RendererStateStacks::ValueType<TStateId>& inValue,
    RendererStateStacks<TRenderer>& ioStateStacks)
{
  if constexpr (TStateId == ERendererStateId::CAMERA) {}
  else if constexpr (TStateId == ERendererStateId::MODEL_MATRIX)
  {
  }
  else if constexpr (TStateId == ERendererStateId::MATERIAL)
  {
  }
  else if constexpr (TStateId == ERendererStateId::OVERRIDE_SHADER_PROGRAM)
  {
  }
  else if constexpr (TStateId == ERendererStateId::RENDER_TEXTURE)
  {
    ioStateStacks.mRenderer.SetRenderTexture(inValue);
  }
  else if constexpr (TStateId == ERendererStateId::DEPTH_ENABLED)
  {
    GL::SetEnabled(GL::Enablable::DEPTH_TEST, inValue);
  }
  else if constexpr (TStateId == ERendererStateId::CULL_FACE_ENABLED)
  {
    GL::SetEnabled(GL::Enablable::CULL_FACE, inValue);
  }
  else if constexpr (TStateId == ERendererStateId::BLEND_ENABLED)
  {
    GL::SetEnabled(GL::Enablable::BLEND, inValue);
  }
  else if constexpr (TStateId == ERendererStateId::BLEND_SOURCE_FACTOR)
  {
    GL::BlendFunc(inValue, ioStateStacks.GetCurrent<ERendererStateId::BLEND_DEST_FACTOR>());
  }
  else if constexpr (TStateId == ERendererStateId::BLEND_DEST_FACTOR)
  {
    GL::BlendFunc(ioStateStacks.GetCurrent<ERendererStateId::BLEND_SOURCE_FACTOR>(), inValue);
  }
  else if constexpr (TStateId == ERendererStateId::POINT_SIZE)
  {
    GL::PointSize(inValue);
  }
  else if constexpr (TStateId == ERendererStateId::LINE_WIDTH)
  {
    GL::LineWidth(inValue);
  }
  else if constexpr (TStateId == ERendererStateId::SCENE_AMBIENT_COLOR)
  {
  }
  else if constexpr (TStateId == ERendererStateId::POINT_LIGHTS)
  {
  }
  else if constexpr (TStateId == ERendererStateId::DIRECTIONAL_LIGHTS)
  {
  }
  else
  {
    static_assert((static_cast<int>(TStateId) == -1), "ApplyState not implemented for this state id.");
  }
}

template <typename TRenderer>
template <ERendererStateId TStateId>
typename RendererStateStacks<TRenderer>::template ValueType<TStateId> RendererStateStacks<TRenderer>::GetDefaultValue()
{
  if constexpr (TStateId == ERendererStateId::CAMERA)
  {
    std::unique_ptr<Camera> default_camera = std::make_unique<PerspectiveCamera>();
    return default_camera;
  }
  else if constexpr (TStateId == ERendererStateId::MODEL_MATRIX)
  {
    return Identity<Mat4f>();
  }
  else if constexpr (TStateId == ERendererStateId::MATERIAL)
  {
    return Material {};
  }
  else if constexpr (TStateId == ERendererStateId::OVERRIDE_SHADER_PROGRAM)
  {
    return nullptr;
  }
  else if constexpr (TStateId == ERendererStateId::RENDER_TEXTURE)
  {
    return nullptr;
  }
  else if constexpr (TStateId == ERendererStateId::DEPTH_ENABLED)
  {
    return true;
  }
  else if constexpr (TStateId == ERendererStateId::CULL_FACE_ENABLED)
  {
    return true;
  }
  else if constexpr (TStateId == ERendererStateId::BLEND_ENABLED)
  {
    return true;
  }
  else if constexpr (TStateId == ERendererStateId::BLEND_SOURCE_FACTOR)
  {
    return GL::EBlendFactor::SRC_ALPHA;
  }
  else if constexpr (TStateId == ERendererStateId::BLEND_DEST_FACTOR)
  {
    return GL::EBlendFactor::ONE_MINUS_SRC_ALPHA;
  }
  else if constexpr (TStateId == ERendererStateId::POINT_SIZE)
  {
    return 5.0f;
  }
  else if constexpr (TStateId == ERendererStateId::LINE_WIDTH)
  {
    return 1.0f;
  }
  else if constexpr (TStateId == ERendererStateId::SCENE_AMBIENT_COLOR)
  {
    return WithValue(White<Color3f>(), 0.3f);
  }
  else if constexpr (TStateId == ERendererStateId::POINT_LIGHTS)
  {
    return {};
  }
  else if constexpr (TStateId == ERendererStateId::DIRECTIONAL_LIGHTS)
  {
    return {};
  }
  else
  {
    static_assert((static_cast<int>(TStateId) == -1), "GetDefaultValue not implemented for this state id.");
  }
  return {};
}
}