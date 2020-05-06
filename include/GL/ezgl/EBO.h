#pragma once

#include "ezgl/Buffer.h"

namespace ez
{
class EBO final : public Buffer<GL::EBufferType::EBO>
{
public:
  using GLGuardType = GLBindGuard<GL::EBindingType::EBO>;

  EBO();
  template <typename T>
  explicit EBO(const Span<T>& inData);
};
}

#include "ezgl/EBO.tcc"
