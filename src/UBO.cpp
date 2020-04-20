#include "UBO.h"

namespace egl
{
UBO::UBO() : Buffer<GL::EBindingType::UBO, GL::EBufferType::UBO>() {}

void UBO::BindToBindingPoint(const GL::Id inBindingPoint)
{
  GL::BindBufferBase(GL::EBufferType::UNIFORM_BUFFER, inBindingPoint, GetGLId());
}
}