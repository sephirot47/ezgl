#pragma once

#include "GL.h"

namespace egl
{
class UBO
{
public:
  UBO();

  template <typename T>
  explicit UBO(const Span<T>& inData);

  void Bind() const;
  void UnBind() const;

  void BufferData();
  template <typename T>
  void BufferData(const Span<T>& inData);

  void BindBufferBase(const GL::Id inBindingPoint);

private:
  GL::Id mGLId = 0;
};
}

#include "UBO.tcc"