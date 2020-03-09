#include "VBO.h"

#include "GL.h"
#include "Macros.h"

namespace egl
{
VBO::VBO()
{
    GL_SAFE_CALL(glGenBuffers(1, &mGLId));
    ENSURES(mGLId > 0);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &mGLId);
}

void VBO::Bind() const
{
    GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, mGLId));
}

void VBO::UnBind() const
{
    GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

bool VBO::IsBound() const
{
    const auto bound_id = GetBoundGLId();
    return bound_id != 0 && bound_id == mGLId;
}

GLId VBO::GetBoundGLId()
{
    GLint bound_id = 0;
    GL_SAFE_CALL(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &bound_id));
    return static_cast<GLId>(bound_id);
}

}