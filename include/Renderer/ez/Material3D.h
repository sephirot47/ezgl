#pragma once

#include "ez/Color.h"
#include "ez/GLGuard.h"
#include "ez/MathInitializers.h"
#include <memory>

namespace ez
{
class Texture2D;
class ShaderProgram;

class Material3D final
{
public:
  using GLGuardType = GLNoOpGuard;

  void SetDiffuseColor(const Color4f& inDiffuseColor);
  Color4f GetDiffuseColor() const;

  void SetSpecularIntensity(const float inSpecularIntensity);
  float GetSpecularIntensity() const;

  void SetSpecularExponent(const float inSpecularExponent);
  float GetSpecularExponent() const;

  void SetTexture(const std::shared_ptr<Texture2D>& inTexture);
  const std::shared_ptr<Texture2D> GetTexture() const;

  void SetLightingEnabled(const bool inLightingEnabled);
  bool IsLightingEnabled() const;

  void Bind(ShaderProgram& ioShaderProgram);

private:
  std::shared_ptr<Texture2D> mTexture = nullptr;

  bool mLightingEnabled = true;
  Color4f mDiffuseColor = White<Color4f>();
  float mSpecularIntensity = 1.0f;
  float mSpecularExponent = 60.0f;
};

}