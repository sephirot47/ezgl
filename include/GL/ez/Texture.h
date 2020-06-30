#pragma once

#include "ez/GLBindableObject.h"
#include "ez/GLGuard.h"

namespace ez
{
template <GL::ETextureTarget TTextureTarget>
class Texture : public GLBindableObject<GL::GetBindingType<TTextureTarget>()>
{
public:
  static constexpr auto TextureTarget = TTextureTarget;
  static constexpr auto BindingType = GL::GetBindingType<TTextureTarget>();

  Texture();
  Texture(Texture&& ioRHS) noexcept = default;
  virtual ~Texture() override = default;

  void BindToTextureUnit(const GL::Size& inTextureUnit) const;
  void BindImageTexture(const GL::Uint inImageUnit,
      const GL::EAccess inAccess,
      const GL::Int inLevel = 0,
      const GL::Boolean inLayered = false,
      const GL::Int inLayer = 0);

  void SetWrapMode(const GL::EWrapMode inWrapMode);
  void SetWrapModeR(const GL::EWrapMode inWrapMode);
  void SetWrapModeS(const GL::EWrapMode inWrapMode);
  void SetWrapModeT(const GL::EWrapMode inWrapMode);

  void SetMinFilterMode(const GL::EMinFilterMode inMinFilterMode);
  void SetMagFilterMode(const GL::EMagFilterMode inMagFilterMode);

  void GenerateMipMap();

  GL::ETextureFormat GetFormat() const { return mFormat; }

  using GLBindableObject<BindingType>::GetGLId;
  GL::ETextureTarget GetTextureTarget() const { return TTextureTarget; }

protected:
  GL::ETextureFormat mFormat = GL::ETextureFormat::RED;
};

}

#include "ez/Texture.tcc"