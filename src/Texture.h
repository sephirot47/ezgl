#pragma once

#include "GLBindableObject.h"
#include "GLGuard.h"

namespace egl
{
template <GL::ETextureTarget TTextureTarget>
class Texture : public GLBindableObject<GL::GetBindingType<TTextureTarget>()>
{
public:
  static constexpr auto TextureTarget = TTextureTarget;
  static constexpr auto BindingType = GL::GetBindingType<TTextureTarget>();

  Texture() = default;
  Texture(Texture&& ioRHS) noexcept = default;
  virtual ~Texture() override = default;

  void BindToTextureUnit(const GL::Size& inTextureUnit) const;

  void GenerateMipMap();

  using GLBindableObject<BindingType>::GetGLId;
  GL::ETextureTarget GetTextureTarget() const { return TTextureTarget; }
};

}

#include "Texture.tcc"