#include "UBO.h"

namespace egl
{
UBO::UBO() : Buffer(GL::EBufferType::UNIFORM_BUFFER) {}

void UBO::BindToBindingPoint(const GL::Id inBindingPoint)
{
  GL::BindBufferBase(GL::EBufferType::UNIFORM_BUFFER, inBindingPoint, GetGLId());
}
}