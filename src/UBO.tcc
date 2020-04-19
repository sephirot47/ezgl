#include "UBO.h"

namespace egl
{
template <typename T>
UBO::UBO(const Span<T>& inData) : UBO()
{
  BufferData(inData);
}

template <typename T>
void UBO::BufferData(const Span<T>& inData)
{
  GL::BufferData(mGLId, inData, GL::EAccessHint::STATIC_DRAW);
}
}