#include "VAO.h"

#include <GL/glew.h>

#include "EBO.h"
#include "VBO.h"

namespace egl
{
VAO::VAO()
{
    glGenVertexArrays(1, &mGLId);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &mGLId);
}

void VAO::Bind() const
{
    glBindVertexArray(mGLId);
}

void VAO::UnBind() const
{
    glBindVertexArray(0);
}

void VAO::AddVBO(const VBO& inVBO, uint32_t inAttribIndex, const VAOVertexAttrib& inVertexAttrib)
{
    Bind();
    inVBO.Bind();
    AddVertexAttrib(inAttribIndex, inVertexAttrib);
}

void VAO::SetEBO(const EBO& inEBO)
{
    Bind();
    inEBO.Bind();
}

void VAO::AddVertexAttrib(uint32_t inAttribIndex, const VAOVertexAttrib& inVertexAttrib)
{
    glEnableVertexAttribArray(inAttribIndex);
    glVertexAttribPointer(inAttribIndex,
        inVertexAttrib.numComponents,
        static_cast<GLenum>(inVertexAttrib.type),
        inVertexAttrib.normalized,
        inVertexAttrib.stride,
        reinterpret_cast<const void*>(inVertexAttrib.offset));
}

void VAO::RemoveVertexAttrib(uint32_t inAttribIndex)
{
    glDisableVertexAttribArray(inAttribIndex);
}
}
