#include "VAO.h"

#include <GL/glew.h>

#include "EBO.h"

#include "GL.h"
#include "Macros.h"
#include "VBO.h"

namespace egl
{
VAO::VAO()
{
    GL_SAFE_CALL(glGenVertexArrays(1, &mGLId));
    ENSURES(mGLId > 0);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &mGLId);
}

void VAO::Bind() const
{
    GL_SAFE_CALL(glBindVertexArray(mGLId));
}

void VAO::UnBind() const
{
    GL_SAFE_CALL(glBindVertexArray(0));
}

bool VAO::IsBound() const
{
    const auto bound_id = GetBoundGLId();
    return bound_id != 0 && bound_id == mGLId;
}

GLId VAO::GetBoundGLId()
{
    GLint bound_id = 0;
    GL_SAFE_CALL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &bound_id));
    return static_cast<GLId>(bound_id);
}

void VAO::AddVBO(const VBO& inVBO, GLId inAttribLocation, const VAOVertexAttrib& inVertexAttrib)
{
    EXPECTS(IsBound());
    inVBO.Bind();
    AddVertexAttrib(inAttribLocation, inVertexAttrib);
}

void VAO::SetEBO(const EBO& inEBO)
{
    EXPECTS(IsBound());
    inEBO.Bind();
}

void VAO::AddVertexAttrib(GLId inAttribLocation, const VAOVertexAttrib& inVertexAttrib)
{
    EXPECTS(IsBound());
    EXPECTS(inVertexAttrib.numComponents > 0);
    GL_SAFE_CALL(glEnableVertexAttribArray(inAttribLocation));
    GL_SAFE_CALL(glVertexAttribPointer(inAttribLocation,
        inVertexAttrib.numComponents,
        static_cast<GLenum>(inVertexAttrib.type),
        inVertexAttrib.normalized,
        inVertexAttrib.stride,
        reinterpret_cast<const void*>(inVertexAttrib.offset)));
}

void VAO::RemoveVertexAttrib(GLId inAttribLocation)
{
    EXPECTS(IsBound());
    EXPECTS(inAttribLocation > 0);
    GL_SAFE_CALL(glDisableVertexAttribArray(inAttribLocation));
}
}
