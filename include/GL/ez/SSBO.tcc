#include <ez/SSBO.h>

namespace ez
{
template <typename T>
SSBO::SSBO(const Span<T>& inData) : SSBO()
{
  BufferData(inData);
}
}