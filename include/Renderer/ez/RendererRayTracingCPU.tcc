#include "ez/RendererRayTracingCPU.h"

namespace ez
{
template <RendererRayTracingCPU::EStateId StateId>
void RendererRayTracingCPU::ApplyState(const State::ValueType<StateId>& inValue, State& ioState)
{
  if constexpr (StateId == RendererRayTracingCPU::EStateId::CAMERA) {}
  else
  {
    static_assert((static_cast<int>(StateId) == -1), "ApplyState not implemented for this state id.");
  }
}

template <RendererRayTracingCPU::EStateId StateId>
typename RendererRayTracingCPU::State::template ValueType<StateId> RendererRayTracingCPU::GetDefaultValue()
{
  if constexpr (StateId == RendererRayTracingCPU::EStateId::CAMERA)
  {
    return std::make_shared<PerspectiveCameraf>();
  }
  else
  {
    static_assert((static_cast<int>(StateId) == -1), "GetDefaultValue not implemented for this state id.");
  }
  return {};
}
}
