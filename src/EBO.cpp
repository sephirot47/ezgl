#include "EBO.h"

#include "GL.h"
#include "Macros.h"

namespace egl
{
EBO::EBO()
    : mGLId(GL::CreateBuffer())
{
    if (mGLId == 0)
        THROW_EXCEPTION("Error creating EBO");
}

EBO::~EBO()
{
    GL::DeleteBuffer(mGLId);
}

void EBO::Bind() const
{
    GL::BindBuffer(GL::EBufferType::ELEMENT_ARRAY, mGLId);
}

void EBO::UnBind() const
{
    GL::BindBuffer(GL::EBufferType::ELEMENT_ARRAY, 0);
}

bool EBO::IsBound() const
{
    const auto bound_id = GetBoundGLId();
    return bound_id != 0 && bound_id == mGLId;
}

GL::Id EBO::GetGLId() const
{
    return mGLId;
}

GL::Id EBO::GetBoundGLId()
{
    const auto bound_id = GL::GetInteger(GL::EBufferBindingType::ELEMENT_ARRAY);
    return static_cast<GL::Id>(bound_id);
}
}