#include "ez/UBO.h"

namespace ez
{
template <typename T>
UBO::UBO(const Span<T>& inData) : UBO()
{
  BufferData(inData);
}
}