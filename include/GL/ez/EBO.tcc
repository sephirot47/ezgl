#include "ez/EBO.h"

namespace ez
{
template <typename T>
EBO::EBO(const Span<T>& inData) : EBO()
{
  BufferData(inData);
}
}