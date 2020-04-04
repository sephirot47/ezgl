#include "VBO.h"

#include "GL.h"
#include "Macros.h"

namespace egl
{
VBO::VBO()
    : mGLId(GL::CreateBuffer())
{
    if (mGLId == 0)
        THROW_EXCEPTION("Error creating VBO");
}

VBO::~VBO()
{
    GL::DeleteBuffer(mGLId);
}

void VBO::Bind() const
{
    GL_SAFE_CALL(GL::BindBuffer(GL::EBufferType::ARRAY_BUFFER, mGLId));
}

void VBO::UnBind() const
{
    GL_SAFE_CALL(GL::BindBuffer(GL::EBufferType::ARRAY_BUFFER, 0));
}

bool VBO::IsBound() const
{
    const auto bound_id = GetBoundGLId();
    return bound_id != 0 && bound_id == mGLId;
}

GL::Id VBO::GetBoundGLId()
{
    const auto bound_id = GL::GetInteger(GL::EBufferBindingType::ARRAY_BUFFER);
    return static_cast<GL::Id>(bound_id);
}

}