#pragma once

#include "Buffer.h"

namespace egl
{

class VBO : public Buffer<GL::EBufferType::VBO>
{
public:
  VBO();
  template <typename T>
  explicit VBO(const Span<T>& inData);
};
}

#include "VBO.tcc"
