#pragma once

#include "ezgl/Buffer.h"

namespace egl
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

#include "ezgl/VBO.tcc"
