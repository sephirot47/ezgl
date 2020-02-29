#include "VBO.h"

#include <GL/glew.h>

namespace egl
{
VBO::VBO()
{
    glGenBuffers(1, &mGLId);
}
VBO::VBO(const void* inData, std::size_t inSize)
{
    BufferData(inData, inSize);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &mGLId);
}

void VBO::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, mGLId);
}

void VBO::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::BufferData(const void* inData, std::size_t inSize)
{
    Bind();
    glBufferData(GL_ARRAY_BUFFER, inSize, inData, GL_STATIC_DRAW);
    UnBind();
}
}