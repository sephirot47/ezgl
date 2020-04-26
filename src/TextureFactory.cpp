#include "TextureFactory.h"
#include "Image2D.h"
#include "Math.h"
#include "Texture2D.h"

namespace egl
{
std::shared_ptr<Texture2D> TextureFactory::GetOneTexture()
{
  static std::shared_ptr<Texture2D> one_texture;
  if (!one_texture)
  {
    const Image2D<Color4f> one_image(1, 1, White<Color4f>());
    one_texture = std::make_shared<Texture2D>(one_image);
  }
  return one_texture;
}

std::shared_ptr<Texture2D> TextureFactory::GetZeroTexture()
{
  static std::shared_ptr<Texture2D> zero_texture;
  if (!zero_texture)
  {
    const Image2D<Color4f> zero_image(1, 1, Zero<Color4f>());
    zero_texture = std::make_shared<Texture2D>(zero_image);
  }
  return zero_texture;
}
}