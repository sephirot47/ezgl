#include "Texture2D.h"
#include "GL.h"
#include "Image2D.h"

namespace egl
{
Texture2D::Texture2D() : Texture(GL::ETextureTarget::TEXTURE_2D)
{
  Bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture2D::Texture2D(const Image2D<Color4f>& inImage2D) : Texture2D() { SetData(inImage2D); }

void Texture2D::SetData(const Image2D<Color4f>& inImage2D,
    const GL::ETextureInternalFormat& inInternalFormat,
    const GL::Int& inMipMapLevel)
{
  SetData(inImage2D.GetWidth(),
      inImage2D.GetHeight(),
      GL::ETextureInputFormat::RGBA,
      GL::ETextureInputComponentFormat::FLOAT,
      inImage2D.GetData(),
      inInternalFormat,
      inMipMapLevel);
}
}