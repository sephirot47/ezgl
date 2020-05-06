#pragma once

#include "ezgl/Buffer.h"

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

#include "ezgl/UBO.tcc"