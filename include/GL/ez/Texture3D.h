#pragma once

#include "ez/GLTypeTraits.h"
#include "ez/Math.h"
#include "ez/Texture.h"
#include <memory>

namespace ez
{
class Texture3D final : public Texture<GL::ETextureTarget::TEXTURE_3D>
{
public:
  using GLGuardType = GLBindGuard<GL::EBindingType::TEXTURE_3D>;

  Texture3D(const int inWidth, const int inHeight, const int inDepth, const GL::ETextureFormat inFormat);
  Texture3D(Texture3D&& ioRHS) noexcept = default;
  Texture3D& operator=(Texture3D&& ioRHS) = default;
  ~Texture3D() override = default;

  void Resize(const Vec3i& inSize);
  void Resize(const int inWidth, const int inHeight, const int inDepth);

  template <typename T>
  void SetData(const GL::Size inWidth,
      const GL::Size inHeight,
      const GL::Size inDepth,
      const GL::ETextureInputFormat inInputFormat,
      const GL::ETextureInputComponentFormat inInputComponentFormat,
      const Span<T>& inData,
      const GL::ETextureFormat inFormat = GL::ETextureFormat::RGBA8,
      const GL::Int inMipMapLevel = 0);

  void SetEmptyData(const GL::Size inWidth,
      const GL::Size inHeight,
      const GL::Size inDepth,
      const GL::ETextureFormat inFormat = GL::ETextureFormat::RGBA8,
      const GL::Int inMipMapLevel = 0);

  GL::Size GetWidth() const;
  GL::Size GetHeight() const;
  GL::Size GetDepth() const;
  const Vec3i& GetSize() const;

private:
  Vec3i mSize = Zero<Vec3i>();

  Texture3D();
};
}

#include "ez/Texture3D.tcc"