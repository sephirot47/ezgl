#include "EBO.h"

namespace egl
{
template <typename T>
EBO::EBO(const Span<T>& inData)
    : EBO()
{
    BufferData(inData);
}

template <typename T>
void EBO::BufferData(const Span<T>& inData)
{
    GL::BufferData(mGLId, inData, GL::EAccessHint::STATIC_DRAW);
}
}