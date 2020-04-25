#include "Renderer3D.h"

namespace egl
{
template <Renderer3D::EStateId StateId>
void Renderer3D::ApplyState(const State::ValueType<StateId>& inValue, State& ioState)
{
  UNUSED(ioState);

  if constexpr (StateId == Renderer3D::EStateId::CAMERA) {}
  else if constexpr (StateId == Renderer3D::EStateId::MODEL_MATRIX)
  {
  }
  else if constexpr (StateId == Renderer3D::EStateId::MATERIAL)
  {
  }
  else if constexpr (StateId == Renderer3D::EStateId::CULL_FACE_ENABLED)
  {
    GL::SetEnabled(GL::Enablable::CULL_FACE, inValue);
  }
  else if constexpr (StateId == Renderer3D::EStateId::SCENE_AMBIENT_COLOR)
  {
  }
  else if constexpr (StateId == Renderer3D::EStateId::POINT_LIGHTS)
  {
  }
  else if constexpr (StateId == Renderer3D::EStateId::DIRECTIONAL_LIGHTS)
  {
  }
  else
  {
    static_assert((static_cast<int>(StateId) == -1), "ApplyState not implemented for this state id.");
  }
}

template <Renderer3D::EStateId StateId>
typename Renderer3D::State::template ValueType<StateId> Renderer3D::GetDefaultValue()
{
  if constexpr (StateId == Renderer3D::EStateId::CAMERA)
  {
    std::unique_ptr<Camera3f> default_camera = std::make_unique<PerspectiveCameraf>();
    return default_camera;
  }
  else if constexpr (StateId == Renderer3D::EStateId::MODEL_MATRIX)
  {
    return Identity<Mat4f>();
  }
  else if constexpr (StateId == Renderer3D::EStateId::MATERIAL)
  {
    return Material3D {};
  }
  else if constexpr (StateId == Renderer3D::EStateId::CULL_FACE_ENABLED)
  {
    return true;
  }
  else if constexpr (StateId == Renderer3D::EStateId::SCENE_AMBIENT_COLOR)
  {
    return WithValue(White<Color3f>(), 0.3f);
  }
  else if constexpr (StateId == Renderer3D::EStateId::POINT_LIGHTS)
  {
    return {};
  }
  else if constexpr (StateId == Renderer3D::EStateId::DIRECTIONAL_LIGHTS)
  {
    return {};
  }
  else
  {
    static_assert((static_cast<int>(StateId) == -1), "GetDefaultValue not implemented for this state id.");
  }
  return {};
}
}