#include "ez/Texture2D.h"
#include "ez/GL.h"
#include "ez/Image2D.h"

namespace ez
{
Texture2D::Texture2D() : Texture()
{
  SetWrapMode(GL::EWrapMode::CLAMP_TO_EDGE);
  SetMinFilterMode(GL::EMinFilterMode::LINEAR);
  SetMagFilterMode(GL::EMagFilterMode::LINEAR);
}

Texture2D::Texture2D(const int inWidth, const int inHeight, const GL::ETextureFormat inFormat) : Texture2D()
{
  SetEmptyData(inWidth, inHeight, inFormat);
}

void Texture2D::Resize(const Vec2i& inSize) { Resize(inSize[0], inSize[1]); }
void Texture2D::Resize(const int inWidth, const int inHeight)
{
  EXPECTS(inWidth >= 1);
  EXPECTS(inHeight >= 1);

  if (Vec2i(inWidth, inHeight) == mSize)
    return;

  SetEmptyData(inWidth, inHeight, GetFormat(), 0);
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