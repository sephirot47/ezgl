#include "GL.h"

namespace egl
{
template <typename T>
void GL::BufferData(const GL::Id inBufferId, const Span<T>& inData, const GL::EAccessHint inAccessHint)
{
    EXPECTS(inBufferId != 0);
    GL_SAFE_CALL(glNamedBufferData(inBufferId, inData.GetSizeInBytes(), inData.GetData(), GL::EnumCast(inAccessHint)));
}

template <typename T>
GL::Int GL::GetInteger(const T& inGLEnum)
{
    GL::Int result = 0;
    GL_SAFE_CALL(glGetIntegerv(GL::EnumCast(inGLEnum), &result));
    return result;
}

template <typename T>
GL::Enum GL::EnumCast(const T& inGLEnum)
{
    return static_cast<GL::Enum>(inGLEnum);
}
}