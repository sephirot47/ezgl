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

  Texture3D() = default;
  Texture3D(const Vec3i& inSize, const GL::ETextureFormat inFormat) : Texture(inSize, inFormat) {}
  Texture3D(Texture3D&& ioRHS) noexcept = default;
  Texture3D& operator=(Texture3D&& ioRHS) = default;
  ~Texture3D() override = default;

  using Texture::TexImage;
  using Texture::TexImageEmpty;

  GL::Size GetWidth() const { return mSize[0]; }
  GL::Size GetHeight() const { return mSize[1]; }
  GL::Size GetDepth() const { return mSize[2]; }
};
}

#include "ez/Texture3D.tcc"