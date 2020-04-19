#include "Buffer.h"
#include "GL.h"

namespace egl
{
template <typename T>
Buffer::Buffer(const GL::EBufferType inBufferType, const Span<T>& inData) : Buffer(inBufferType)
{
  BufferData(inData);
}

template <typename T>
void Buffer::BufferData(const Span<T>& inData)
{
  GL::BufferData(mGLId, inData, GL::EAccessHint::STATIC_DRAW);
}
}