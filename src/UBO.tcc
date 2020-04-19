#include "UBO.h"

namespace egl
{
template <typename T>
UBO::UBO(const Span<T>& inData) : UBO()
{
  BufferData(inData);
}
}