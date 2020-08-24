#pragma once

#include <ez/GLTypeTraits.h>
#include <ez/Math.h>
#include <ez/Texture.h>
#include <memory>

namespace ez
{
template <typename T>
class Image2D;

class Texture2D final : public Texture<GL::ETextureTarget::TEXTURE_2D>
{
public:
  using Texture = Texture<GL::ETextureTarget::TEXTURE_2D>;
  using GLGuardType = GLBindGuard<GL::EBindingType::TEXTURE_2D>;

  Texture2D() = default;
  Texture2D(const Vec2i& inSize, const GL::ETextureFormat inFormat) : Texture(inSize, inFormat) {}
  template <typename TImageValueType>
  explicit Texture2D(const Image2D<TImageValueType>& inImage2D);
  Texture2D(Texture2D&& ioRHS) noexcept = default;
  Texture2D& operator=(Texture2D&& ioRHS) = default;
  ~Texture2D() override = default;

  template <typename TImageValueType>
  void LoadImage(const Image2D<TImageValueType>& inImage2D, const GL::Int& inMipMapLevel = 0);

  using Texture::TexImage;
  using Texture::TexImageEmpty;

  GL::Size GetWidth() const { return mSize[0]; }
  GL::Size GetHeight() const { return mSize[1]; }

  Image2D<Color4f> GetImage(const bool inInvertY = false, const int inMipmapLevel = 0) const;
};
}

#include "ez/Texture2D.tcc"