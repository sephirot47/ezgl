#include "ez/Texture2D.h"
#include <ez/GL.h>
#include "ez/Image2D.h"

namespace ez
{
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