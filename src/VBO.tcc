#include "GL.h"
#include "VBO.h"

namespace egl
{
template <typename T>
VBO::VBO(const Span<T>& inData) : VBO()
{
  BufferData(inData);
}
}