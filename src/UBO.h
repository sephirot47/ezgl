#pragma once

#include "Buffer.h"

namespace egl
{
class UBO : public Buffer<GL::EBufferType::UBO>
{
public:
  using GLGuardType = GLBindGuard<GL::EBindingType::UBO>;

  UBO();
  template <typename T>
  explicit UBO(const Span<T>& inData);

  void BindToBindingPoint(const GL::Id inBindingPoint);
};
}

#include "UBO.tcc"