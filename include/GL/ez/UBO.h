#pragma once

#include <ez/Buffer.h>

namespace ez
{
class UBO final : public Buffer<GL::EBufferType::UBO>
{
public:
  using GLGuardType = GLBindGuard<GL::EBindingType::UBO>;

  UBO();
  template <typename T>
  explicit UBO(const Span<T>& inData);

  void BindToBindingPoint(const GL::Id inBindingPoint);
};
}

#include "ez/UBO.tcc"