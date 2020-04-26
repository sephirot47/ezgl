#pragma once

#include "Buffer.h"

namespace egl
{

class VBO : public Buffer<GL::EBufferType::VBO>
{
public:
  using GLGuardType = GLBindGuard<GL::EBindingType::VBO>;

  VBO();
  template <typename T>
  explicit VBO(const Span<T>& inData);
};
}

#include "VBO.tcc"
