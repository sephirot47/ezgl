#pragma once

#include <memory>

namespace ez
{
class Texture2D;

class TextureFactory final
{
public:
  static std::shared_ptr<Texture2D> GetOneTexture();
  static std::shared_ptr<Texture2D> GetZeroTexture();
};
}