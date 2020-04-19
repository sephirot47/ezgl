#pragma once

#include "Buffer.h"

namespace egl
{
class UBO : public Buffer
{
public:
  UBO();
  template <typename T>
  explicit UBO(const Span<T>& inData);

  void BindToBindingPoint(const GL::Id inBindingPoint);
};
}

#include "UBO.tcc"