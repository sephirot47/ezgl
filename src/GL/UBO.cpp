#include "ezgl/UBO.h"

namespace ez
{
UBO::UBO() : Buffer<GL::EBufferType::UBO>() {}

void UBO::BindToBindingPoint(const GL::Id inBindingPoint)
{
  GL::BindBufferBase(GL::EBufferType::UBO, inBindingPoint, GetGLId());
}
}