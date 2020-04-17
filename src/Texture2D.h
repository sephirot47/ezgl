#pragma once

#include "Texture.h"

namespace egl
{

class Texture2D : public Texture
{
public:
  Texture2D();
  Texture2D(Texture2D&& ioRHS) = default;
  Texture2D& operator=(Texture2D&& ioRHS) = default;
  ~Texture2D() = default;

  template <typename T>
  void SetData(const GL::Size& inWidth,
      const GL::Size& inHeight,
      const GL::ETextureInputFormat& inInputFormat,
      const GL::ETextureInputComponentFormat& inInputComponentFormat,
      const Span<T>& inData,
      const GL::ETextureInternalFormat& inInternalFormat,
      const GL::Int& inMipMapLevel = 0);
};

}

#include "Texture2D.tcc"