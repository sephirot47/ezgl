#include "ezgl/UBO.h"

namespace egl
{
UBO::UBO() : Buffer<GL::EBufferType::UBO>() {}

void UBO::BindToBindingPoint(const GL::Id inBindingPoint)
{
  GL::BindBufferBase(GL::EBufferType::UBO, inBindingPoint, GetGLId());
}
}