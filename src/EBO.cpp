#include "EBO.h"

#include "GL.h"
#include "Macros.h"

namespace egl
{
EBO::EBO()
{
    GL_SAFE_CALL(glGenBuffers(1, &mGLId));
}

EBO::~EBO()
{
    glDeleteBuffers(1, &mGLId);
}

void EBO::Bind() const
{
    GL_SAFE_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLId));
}

void EBO::UnBind() const
{
    GL_SAFE_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

bool EBO::IsBound() const
{
    const auto bound_id = GetBoundGLId();
    return bound_id != 0 && bound_id == mGLId;
}

GLId EBO::GetBoundGLId()
{
    GLint bound_id = 0;
    GL_SAFE_CALL(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &bound_id));
    return static_cast<GLId>(bound_id);
}
}