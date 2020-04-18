#pragma once

#include "GL.h"

namespace egl
{
class Texture
{
public:
  Texture(const GL::ETextureTarget inTarget);
  Texture(const Texture& inRHS) = delete;
  Texture& operator=(const Texture& inRHS) = delete;
  Texture(Texture&& ioRHS) noexcept;
  Texture& operator=(Texture&& ioRHS) = delete;
  virtual ~Texture();

  void Bind() const;
  void BindToTextureUnit(const GL::Size& inTextureUnit) const;
  void UnBind() const;

  void GenerateMipMap();

  GL::Id GetGLId() const;
  GL::ETextureTarget GetTextureTarget() const;

private:
  GL::Id mGLId = 0;
  const GL::ETextureTarget mTextureTarget = GL::ETextureTarget::TEXTURE_2D;
};

}

#include "Texture.tcc"