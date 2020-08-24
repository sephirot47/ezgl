#pragma once

#include <ez/Buffer.h>

namespace ez
{

class VBO final : public Buffer<GL::EBufferType::VBO>
{
public:
  using GLGuardType = GLBindGuard<GL::EBindingType::VBO>;

  VBO();
  template <typename T>
  explicit VBO(const Span<T>& inData);
};
}

#include "ez/VBO.tcc"
