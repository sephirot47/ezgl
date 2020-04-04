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
    if (mGLId == 0)
        THROW_EXCEPTION("Error creating VAO");
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

GL::Id VAO::GetBoundGLId()
{
    GLint bound_id = 0;
    GL_SAFE_CALL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &bound_id));
    return static_cast<GL::Id>(bound_id);
}

void VAO::AddVBO(const VBO& inVBO, const GL::Id inAttribLocation, const VAOVertexAttrib& inVertexAttrib)
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

void VAO::AddVertexAttrib(const GL::Id inAttribLocation, const VAOVertexAttrib& inVertexAttrib)
{
    EXPECTS(IsBound());
    EXPECTS(inVertexAttrib.mNumComponents > 0);
    GL_SAFE_CALL(glEnableVertexAttribArray(inAttribLocation));
    GL_SAFE_CALL(glVertexAttribPointer(inAttribLocation,
        inVertexAttrib.mNumComponents,
        GL::EnumCast(inVertexAttrib.mType),
        inVertexAttrib.mNormalized,
        inVertexAttrib.mStride,
        reinterpret_cast<const void*>(inVertexAttrib.mOffset)));
}

void VAO::RemoveVertexAttrib(const GL::Id inAttribLocation)
{
    EXPECTS(IsBound());
    EXPECTS(inAttribLocation > 0);
    GL_SAFE_CALL(glDisableVertexAttribArray(inAttribLocation));
}
}
