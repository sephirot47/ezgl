#include "GL.h"
#include "VBO.h"

namespace egl
{
template <typename T>
VBO::VBO(const Span<T>& inData) : VBO()
{
  BufferData(inData);
}

template <typename T>
void VBO::BufferData(const Span<T>& inData)
{
  GL::BufferData(mGLId, inData, GL::EAccessHint::STATIC_DRAW);
}
}