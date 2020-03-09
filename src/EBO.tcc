#include "EBO.h"

namespace egl
{
template <typename T>
EBO::EBO(const T* inData, std::size_t inSize)
    : EBO()
{
    Bind();
    BufferData(inData, inSize);
    UnBind();
}

template <typename T>
void EBO::BufferData(const T* inData, std::size_t inSize)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, inSize, inData, GL_STATIC_DRAW));
}
}