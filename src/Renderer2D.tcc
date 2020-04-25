#include "Renderer2D.h"

namespace egl
{
template <Renderer2D::EStateId StateId>
void Renderer2D::ApplyState(const State::ValueType<StateId>& inValue, State& ioState)
{
  UNUSED(inValue);
  UNUSED(ioState);

  if constexpr (StateId == Renderer2D::EStateId::CAMERA) {}
  else if constexpr (StateId == Renderer2D::EStateId::MODEL_MATRIX)
  {
  }
  else if constexpr (StateId == Renderer2D::EStateId::MATERIAL)
  {
  }
  else
  {
    static_assert((static_cast<int>(StateId) == -1), "ApplyState not implemented for this state id.");
  }
}

template <Renderer2D::EStateId StateId>
typename Renderer2D::State::template ValueType<StateId> Renderer2D::GetDefaultValue()
{
  if constexpr (StateId == Renderer2D::EStateId::CAMERA)
  {
    std::unique_ptr<OrthographicCamera2f> default_camera = std::make_unique<OrthographicCamera2f>();
    return default_camera;
  }
  else if constexpr (StateId == Renderer2D::EStateId::MODEL_MATRIX)
  {
    return Identity<Mat3f>();
  }
  else if constexpr (StateId == Renderer2D::EStateId::MATERIAL)
  {
    return Material2D {};
  }
  else
  {
    static_assert((static_cast<int>(StateId) == -1), "GetDefaultValue not implemented for this state id.");
  }
  return {};
}
}