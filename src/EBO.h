#pragma once

#include "Buffer.h"

namespace egl
{
class EBO : public Buffer
{
public:
  EBO();
  template <typename T>
  explicit EBO(const Span<T>& inData);
};
}

#include "EBO.tcc"
