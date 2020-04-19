#include "UBO.h"

namespace egl
{
UBO::UBO() {}

void UBO::Bind() const { GL::BindBuffer(GL::EBufferType::UNIFORM_BUFFER, mGLId); }
void UBO::UnBind() const { GL::BindBuffer(GL::EBufferType::UNIFORM_BUFFER, 0); }

void UBO::BindBufferBase(const GL::Id inBindingPoint)
{
  GL::BindBufferBase(GL::EBufferType::UNIFORM_BUFFER, inBindingPoint, mGLId);
}
}