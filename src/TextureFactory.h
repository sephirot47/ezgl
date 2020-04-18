#pragma once

#include <memory>

namespace egl
{
class Texture2D;

class TextureFactory
{
public:
  static std::shared_ptr<Texture2D> GetWhiteTexture();
};
}