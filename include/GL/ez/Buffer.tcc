#include "ez/Buffer.h"
#include "ez/GL.h"

namespace ez
{
template <GL::EBufferType TBufferType>
template <typename T>
Buffer<TBufferType>::Buffer(const Span<T>& inData) : Buffer()
{
  BufferData(inData);
}

template <GL::EBufferType TBufferType>
void Buffer<TBufferType>::BufferDataEmpty(const GL::Size inSizeInBytes, const GL::EAccessHint inAccessHint)
{
  BufferData(Span<uint8_t>(nullptr, inSizeInBytes), inAccessHint);
}

template <GL::EBufferType TBufferType>
template <typename T>
void Buffer<TBufferType>::BufferData(const Span<T>& inData, const GL::EAccessHint inAccessHint)
{
  GL::BufferData(GetGLId(), inData, inAccessHint);
  mInitialized = true;
}

template <GL::EBufferType TBufferType>
template <typename T>
void Buffer<TBufferType>::BufferSubData(const Span<T>& inData, const GL::Size inOffset)
{
  if (!mInitialized)
    THROW_EXCEPTION("Before using BufferSubData, you must initialize the Buffer by calling BufferData");

  GL::BufferSubData(GetGLId(), inData, inOffset);
}

template <GL::EBufferType TBufferType>
void* Buffer<TBufferType>::MapBuffer(const GL::EAccess inAccess)
{
  return GL::MapBuffer(GetGLId(), inAccess);
}

template <GL::EBufferType TBufferType>
void* Buffer<TBufferType>::MapBufferRange(const std::size_t inOffset,
    const std::size_t inLength,
    const GL::EAccessBitFlags inAccessBitFlags)
{
  return GL::MapBufferRange(GetGLId(), inOffset, inLength, inAccessBitFlags);
}

template <GL::EBufferType TBufferType>
void Buffer<TBufferType>::UnmapBuffer()
{
  GL::UnmapBuffer(GetGLId());
}
}