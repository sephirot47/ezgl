#pragma once

#include "GLGuard.h"
#include "Color.h"
#include "MathInitializers.h"
#include <memory>

namespace egl
{
class Texture2D;
class ShaderProgram;

class Material2D final
{
public:
  using GLGuardType = GLNoOpGuard;

  void SetColor(const Color4f& inColor) { mColor = inColor; }
  Color4f GetColor() const { return mColor; }

  void SetTexture(const std::shared_ptr<Texture2D>& inTexture);
  const std::shared_ptr<Texture2D> GetTexture() const;

  void Bind(ShaderProgram& ioShaderProgram);

private:
  std::shared_ptr<Texture2D> mTexture;
  Color4f mColor = White<Color4f>();
};

}