#include "VBO.h"

#include "GL.h"

namespace egl
{
template <typename T>
VBO::VBO(const T* inData, std::size_t inSize)
    : VBO()
{
    Bind();
    BufferData(inData, inSize);
}

template <typename T>
void VBO::BufferData(const T* inData, std::size_t inSize)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glBufferData(GL_ARRAY_BUFFER, inSize, inData, GL_STATIC_DRAW));
}
}