#include "ez/GL.h"
#include "ez/VBO.h"

namespace ez
{
template <typename T>
VBO::VBO(const Span<T>& inData) : VBO()
{
  BufferData(inData);
}
}