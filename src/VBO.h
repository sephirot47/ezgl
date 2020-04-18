#pragma once

#include "GL.h"
#include "Span.h"
#include <cstdint>

namespace egl
{

class VBO
{
public:
  VBO();
  template <typename T>
  VBO(const Span<T>& inData);
  VBO(const VBO& inRHS) = delete;
  VBO& operator=(const VBO& inRHS) = delete;
  VBO(VBO&& ioRHS) noexcept;
  VBO& operator=(VBO&& ioRHS) = delete;
  ~VBO();

  void Bind() const;
  void UnBind() const;
  bool IsBound() const;
  static GL::Id GetBoundGLId();

  template <typename T>
  void BufferData(const Span<T>& inData);

  GL::Id GetGLId() const { return mGLId; }

private:
  GL::Id mGLId = 0;
};
}

#include "VBO.tcc"
