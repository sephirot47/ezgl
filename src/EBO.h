#pragma once

#include "Buffer.h"

namespace egl
{
class EBO : public Buffer<GL::EBufferType::EBO>
{
public:
  using GLGuardType = GLBindGuard<GL::EBindingType::EBO>;

  EBO();
  template <typename T>
  explicit EBO(const Span<T>& inData);
};
}

#include "EBO.tcc"
