#include <ez/SSBO.h>

namespace ez
{
SSBO::SSBO() : Buffer<GL::EBufferType::SSBO>() {}

void SSBO::BindToBindingPoint(const GL::Id inBindingPoint)
{
  GL::BindBufferBase(GL::EBufferType::SSBO, inBindingPoint, GetGLId());
}
}