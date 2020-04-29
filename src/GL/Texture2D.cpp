#include "ezgl/Texture2D.h"
#include "ezgl/GL.h"
#include "ezgl/Image2D.h"

namespace egl
{
Texture2D::Texture2D() : Texture()
{
  const auto texture_bind_guard = BindGuarded();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture2D::Texture2D(const int inWidth, const int inHeight, const GL::ETextureFormat inFormat) : Texture2D()
{
  SetEmptyData(inWidth, inHeight, inFormat);
}

Texture2D::Texture2D(const Image2D<Color4f>& inImage2D) : Texture2D() { SetData(inImage2D); }

void Texture2D::Resize(const Vec2i& inSize) { Resize(inSize[0], inSize[1]); }
void Texture2D::Resize(const int inWidth, const int inHeight)
{
  EXPECTS(inWidth >= 1);
  EXPECTS(inHeight >= 1);

  if (Vec2i(inWidth, inHeight) == mSize)
    return;

  SetEmptyData(inWidth, inHeight, GetFormat(), 0);
}

void Texture2D::SetData(const Image2D<Color4f>& inImage2D,
    const GL::ETextureFormat& inFormat,
    const GL::Int& inMipMapLevel)
{
  SetData(inImage2D.GetWidth(),
      inImage2D.GetHeight(),
      GL::ETextureInputFormat::RGBA,
      GL::ETextureInputComponentFormat::FLOAT,
      inImage2D.GetData(),
      inFormat,
      inMipMapLevel);
}

void Texture2D::SetEmptyData(const GL::Size inWidth,
    const GL::Size inHeight,
    const GL::ETextureFormat& inFormat,
    const GL::Int& inMipMapLevel)
{
  auto texture_input_format = GL::ETextureInputFormat::RED;
  if (GL::IsDepthOnlyFormat(inFormat) || GL::IsDepthStencilFormat(inFormat))
  {
    texture_input_format = GL::ETextureInputFormat::DEPTH_COMPONENT;
  }

  SetData(inWidth,
      inHeight,
      texture_input_format,
      GL::ETextureInputComponentFormat::FLOAT,
      Span<float>(nullptr, 0),
      inFormat,
      inMipMapLevel);
}

GL::Size Texture2D::GetWidth() const { return mSize[0]; }
GL::Size Texture2D::GetHeight() const { return mSize[1]; }
const Vec2i& Texture2D::GetSize() const { return mSize; }
GL::ETextureFormat Texture2D::GetFormat() const { return mFormat; }

Image2D<Color4f> Texture2D::GetImage(const bool inInvertY, const int inMipmapLevel) const
{
  EXPECTS(inMipmapLevel >= 0);

  Image2D<Color4f> texture_image(GetWidth(), GetHeight(), Zero<Color4f>());
  const auto pixels = GL::GetTextureImage<Color4f>(GetGLId(),
      GL::ETextureInputFormat::RGBA,
      GL::EDataType::FLOAT,
      GetWidth() * GetHeight(),
      inMipmapLevel);

  for (GL::Size y = 0; y < GetHeight(); ++y)
  {
    for (GL::Size x = 0; x < GetWidth(); ++x)
    {
      const auto i = ((inInvertY ? (GetHeight() - y - 1) : y) * GetWidth() + x);
      texture_image.Get(x, y) = pixels[i];
    }
  }

  return texture_image;
}
}