#pragma once

#include "Texture.h"
#include <memory>

namespace egl
{
template <typename T>
class Image2D;

class Texture2D : public Texture
{
public:
  Texture2D();
  explicit Texture2D(const Image2D<Color4f>& inImage2D);
  Texture2D(Texture2D&& ioRHS) = default;
  Texture2D& operator=(Texture2D&& ioRHS) = default;
  ~Texture2D() = default;

  template <typename T>
  void SetData(const GL::Size& inWidth,
      const GL::Size& inHeight,
      const GL::ETextureInputFormat& inInputFormat,
      const GL::ETextureInputComponentFormat& inInputComponentFormat,
      const Span<T>& inData,
      const GL::ETextureInternalFormat& inInternalFormat = GL::ETextureInternalFormat::RGBA8,
      const GL::Int& inMipMapLevel = 0);

  void SetData(const Image2D<Color4f>& inImage2D,
      const GL::ETextureInternalFormat& inInternalFormat = GL::ETextureInternalFormat::RGBA8,
      const GL::Int& inMipMapLevel = 0);
};
}

#include "Texture2D.tcc"