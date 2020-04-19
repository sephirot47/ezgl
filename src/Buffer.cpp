#include "Buffer.h"
#include "GL.h"
#include "Macros.h"

namespace egl
{
Buffer::Buffer(const GL::EBufferType inBufferType) : mBufferType(inBufferType), mGLId(GL::CreateBuffer())
{
  if (mGLId == 0)
    THROW_EXCEPTION("Error creating Buffer");
}

Buffer::Buffer(Buffer&& ioRHS) noexcept
{
  EXPECTS(mGLId == 0);
  std::swap(mGLId, ioRHS.mGLId);
}

Buffer::~Buffer()
{
  if (mGLId != 0)
    GL::DeleteBuffer(mGLId);
}

void Buffer::Bind() const { GL::BindBuffer(mBufferType, mGLId); }

void Buffer::UnBind() const { GL::BindBuffer(mBufferType, 0); }

bool Buffer::IsBound() const
{
  const auto bound_id = GetBoundGLId(mBufferType);
  return bound_id != 0 && bound_id == mGLId;
}

GL::Id Buffer::GetBoundGLId(const GL::EBufferType inBufferType)
{
  const auto buffer_binding_type = GL::GetBufferBindingType(inBufferType);
  const auto bound_id = GL::GetInteger(buffer_binding_type);
  return static_cast<GL::Id>(bound_id);
}

void Buffer::BufferDataEmpty(const GL::Size inSizeInBytes) { BufferData(Span<uint8_t>(nullptr, inSizeInBytes)); }

}