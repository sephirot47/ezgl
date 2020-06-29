#pragma once

#include "ez/Buffer.h"

namespace ez
{
class SSBO final : public Buffer<GL::EBufferType::SSBO>
{
public:
  using GLGuardType = GLBindGuard<GL::EBindingType::SSBO>;

  SSBO();
  template <typename T>
  explicit SSBO(const Span<T>& inData);

  void BindToBindingPoint(const GL::Id inBindingPoint);
};
}

#include "ez/SSBO.tcc"