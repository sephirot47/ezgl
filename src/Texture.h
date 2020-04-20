#pragma once

#include "GLBindableObject.h"

namespace egl
{
template <GL::ETextureTarget TTextureTarget, GL::EBindingType TBindingType>
class Texture : public GLBindableObject<TBindingType>
{
  // For the moment only Texture2D supported
  static_assert((TTextureTarget == GL::ETextureTarget::TEXTURE_2D && TBindingType == GL::EBindingType::TEXTURE_2D));

public:
  static constexpr auto TextureTarget = TTextureTarget;

  Texture() = default;
  Texture(Texture&& ioRHS) noexcept = default;
  virtual ~Texture() override = default;

  void BindToTextureUnit(const GL::Size& inTextureUnit) const;

  void GenerateMipMap();

  using GLBindableObject<TBindingType>::GetGLId;
  GL::ETextureTarget GetTextureTarget() const { return TTextureTarget; }
};

}

#include "Texture.tcc"