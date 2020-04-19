#include "EBO.h"

namespace egl
{
template <typename T>
EBO::EBO(const Span<T>& inData) : EBO()
{
  BufferData(inData);
}
}