#include "ezgl/GL.h"
#include "ezgl/VBO.h"

namespace egl
{
template <typename T>
VBO::VBO(const Span<T>& inData) : VBO()
{
  BufferData(inData);
}
}