#pragma once

#include "Buffer.h"

namespace egl
{

class VBO : public Buffer
{
public:
  VBO();
  template <typename T>
  VBO(const Span<T>& inData);
};
}

#include "VBO.tcc"
