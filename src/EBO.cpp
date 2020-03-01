#include "EBO.h"

#include <GL/glew.h>

namespace egl
{
EBO::EBO()
{
    glGenBuffers(1, &mGLId);
}
EBO::EBO(const void* inData, std::size_t inSize)
    : EBO()
{
    BufferData(inData, inSize);
}

EBO::~EBO()
{
    glDeleteBuffers(1, &mGLId);
}

void EBO::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLId);
}

void EBO::UnBind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::BufferData(const void* inData, std::size_t inSize)
{
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, inSize, inData, GL_STATIC_DRAW);
    UnBind();
}
}