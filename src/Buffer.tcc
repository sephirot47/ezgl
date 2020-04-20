#include "Buffer.h"
#include "GL.h"

namespace egl
{
template <GL::EBindingType TBindingType, GL::EBufferType TBufferType>
template <typename T>
Buffer<TBindingType, TBufferType>::Buffer(const Span<T>& inData) : Buffer()
{
  BufferData(inData);
}

template <GL::EBindingType TBindingType, GL::EBufferType TBufferType>
void Buffer<TBindingType, TBufferType>::BufferDataEmpty(const GL::Size inSizeInBytes)
{
  BufferData(Span<uint8_t>(nullptr, inSizeInBytes));
}

template <GL::EBindingType TBindingType, GL::EBufferType TBufferType>
template <typename T>
void Buffer<TBindingType, TBufferType>::BufferData(const Span<T>& inData)
{
  GL::BufferData(GLObject<TBindingType>::GetGLId(), inData, GL::EAccessHint::STATIC_DRAW);
  mInitialized = true;
}

template <GL::EBindingType TBindingType, GL::EBufferType TBufferType>
template <typename T>
void Buffer<TBindingType, TBufferType>::BufferSubData(const Span<T>& inData, const GL::Size inOffset)
{
  if (!mInitialized)
    THROW_EXCEPTION("Before using BufferSubData, you must initialize the Buffer by calling BufferData");

  GL::BufferSubData(GLObject<TBindingType>::GetGLId(), inData, inOffset);
}
}