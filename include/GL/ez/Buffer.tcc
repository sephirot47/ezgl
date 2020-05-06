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
void Buffer<TBufferType>::BufferDataEmpty(const GL::Size inSizeInBytes)
{
  BufferData(Span<uint8_t>(nullptr, inSizeInBytes));
}

template <GL::EBufferType TBufferType>
template <typename T>
void Buffer<TBufferType>::BufferData(const Span<T>& inData)
{
  GL::BufferData(GetGLId(), inData, GL::EAccessHint::STATIC_DRAW);
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
}