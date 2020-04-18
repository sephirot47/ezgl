#include "Texture.h"

namespace egl
{
Texture::Texture(const GL::ETextureTarget inTarget) : mGLId(GL::CreateTexture(inTarget)), mTextureTarget(inTarget)
{
  if (mGLId == 0)
    THROW_EXCEPTION("Error creating Texture");
}

Texture::Texture(Texture&& ioRHS) noexcept
{
  EXPECTS(mGLId == 0);
  std::swap(mGLId, ioRHS.mGLId);
}

Texture::~Texture()
{
  if (mGLId != 0)
    GL::DeleteBuffer(mGLId);
}

void Texture::Bind() const { GL::BindTexture(mTextureTarget, mGLId); }

void Texture::BindToTextureUnit(const GL::Size& inTextureUnit) const
{
  EXPECTS((inTextureUnit + GL_TEXTURE0) <= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
  GL::BindTextureUnit(inTextureUnit, mGLId);
}

void Texture::UnBind() const { GL::BindTexture(mTextureTarget, 0); }

void Texture::GenerateMipMap() { GL::GenerateTextureMipMap(mGLId); }

GL::Id Texture::GetGLId() const { return mGLId; }

GL::ETextureTarget Texture::GetTextureTarget() const { return mTextureTarget; }
}