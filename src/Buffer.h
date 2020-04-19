#pragma once

#include "GL.h"
#include "Span.h"

namespace egl
{
class Buffer
{
public:
  Buffer(const GL::EBufferType inBufferType);
  template <typename T>
  Buffer(const GL::EBufferType inBufferType, const Span<T>& inData);
  Buffer(const Buffer& inRHS) = delete;
  Buffer& operator=(const Buffer& inRHS) = delete;
  Buffer(Buffer&& ioRHS) noexcept;
  Buffer& operator=(Buffer&& ioRHS) = delete;
  ~Buffer();

  void Bind() const;
  void UnBind() const;
  bool IsBound() const;
  static GL::Id GetBoundGLId(const GL::EBufferType inBufferType);

  void BufferDataEmpty(const GL::Size inSizeInBytes);
  template <typename T>
  void BufferData(const Span<T>& inData);
  template <typename T>
  void BufferSubData(const Span<T>& inData, const GL::Size inOffset = 0);

  GL::Id GetGLId() const { return mGLId; }

private:
  const GL::EBufferType mBufferType = GL::EBufferType::ARRAY_BUFFER;
  bool mInitialized = false;
  GL::Id mGLId = 0;
};
}

#include "Buffer.tcc"