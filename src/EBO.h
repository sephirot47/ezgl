#pragma once

#include "GL.h"
#include "Span.h"
#include <cstdint>

namespace egl
{

class EBO
{
public:
  EBO();
  template <typename T>
  explicit EBO(const Span<T>& inData);
  EBO(const EBO& inRHS) = delete;
  EBO& operator=(const EBO& inRHS) = delete;
  EBO(EBO&& ioRHS) noexcept;
  EBO& operator=(EBO&& ioRHS) = delete;
  ~EBO();

  void Bind() const;
  void UnBind() const;
  bool IsBound() const;
  static GL::Id GetBoundGLId();

  template <typename T>
  void BufferData(const Span<T>& inData);

  GL::Id GetGLId() const;

private:
  GL::Id mGLId = 0;
};
}

#include "EBO.tcc"
