#pragma once

#include "Math.h"
#include "Texture.h"
#include <memory>

namespace egl
{
template <typename T>
class Image2D;

class Texture2D : public Texture<GL::ETextureTarget::TEXTURE_2D>
{
public:
  using GLGuardType = GLBindGuard<GL::EBindingType::TEXTURE_2D>;

  Texture2D(const int inWidth, const int inHeight, const GL::ETextureFormat inFormat);
  explicit Texture2D(const Image2D<Color4f>& inImage2D);
  Texture2D(Texture2D&& ioRHS) noexcept = default;
  Texture2D& operator=(Texture2D&& ioRHS) = default;
  ~Texture2D() override = default;

  void Resize(const Vec2i& inSize);
  void Resize(const int inWidth, const int inHeight);

  template <typename T>
  void SetData(const GL::Size& inWidth,
      const GL::Size& inHeight,
      const GL::ETextureInputFormat& inInputFormat,
      const GL::ETextureInputComponentFormat& inInputComponentFormat,
      const Span<T>& inData,
      const GL::ETextureFormat& inFormat = GL::ETextureFormat::RGBA8,
      const GL::Int& inMipMapLevel = 0);

  void SetData(const Image2D<Color4f>& inImage2D,
      const GL::ETextureFormat& inFormat = GL::ETextureFormat::RGBA8,
      const GL::Int& inMipMapLevel = 0);

  void SetEmptyData(const GL::Size inWidth,
      const GL::Size inHeight,
      const GL::ETextureFormat& inFormat = GL::ETextureFormat::RGBA8,
      const GL::Int& inMipMapLevel = 0);

  GL::Size GetWidth() const;
  GL::Size GetHeight() const;
  const Vec2i& GetSize() const;
  GL::ETextureFormat GetFormat() const;

  Image2D<Color4f> GetImage(const bool inInvertY = false, const int inMipmapLevel = 0) const;

private:
  Vec2i mSize = Zero<Vec2i>();
  GL::ETextureFormat mFormat = GL::ETextureFormat::RED;

  Texture2D();
};
}

#include "Texture2D.tcc"