#pragma once

#include "GL.h"
#include "GLObject.h"
#include "Span.h"

namespace egl
{
template <GL::EBindingType TBindingType, GL::EBufferType TBufferType>
class Buffer : public GLObject<TBindingType>
{
public:
  static constexpr auto BufferType = TBufferType;

  Buffer() = default;
  template <typename T>
  explicit Buffer(const Span<T>& inData);
  Buffer(Buffer&& ioRHS) noexcept = default;
  virtual ~Buffer() = default;

  void BufferDataEmpty(const GL::Size inSizeInBytes);
  template <typename T>
  void BufferData(const Span<T>& inData);
  template <typename T>
  void BufferSubData(const Span<T>& inData, const GL::Size inOffset = 0);

private:
  bool mInitialized = false;
};
}

#include "Buffer.tcc"