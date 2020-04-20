#include "TextureFactory.h"
#include "Image2D.h"
#include "Math.h"
#include "Texture2D.h"

namespace egl
{
std::shared_ptr<Texture2D> TextureFactory::GetWhiteTexture()
{
  static std::shared_ptr<Texture2D> white_texture = nullptr;
  if (!white_texture)
  {
    const Image2D<Color4f> white_image(1, 1, White<Color4f>());

    white_texture = std::make_shared<Texture2D>(white_image);
  }
  return white_texture;
}
}