#include "VBO.h"
#include "GL.h"
#include "Macros.h"

namespace egl
{
VBO::VBO() : mGLId(GL::CreateBuffer())
{
  if (mGLId == 0)
    THROW_EXCEPTION("Error creating VBO");
}

VBO::VBO(VBO&& ioRHS) noexcept { *this = std::move(ioRHS); }

VBO& VBO::operator=(VBO&& ioRHS) noexcept
{
  if (this == &ioRHS)
    return *this;

  if (mGLId != 0)
    GL::DeleteVertexArray(mGLId);

  mGLId = ioRHS.mGLId;
  ioRHS.mGLId = 0;
  return *this;
}

VBO::~VBO()
{
  if (mGLId != 0)
    GL::DeleteBuffer(mGLId);
}

void VBO::Bind() const { GL::BindBuffer(GL::EBufferType::ARRAY_BUFFER, mGLId); }

void VBO::UnBind() const { GL::BindBuffer(GL::EBufferType::ARRAY_BUFFER, 0); }

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