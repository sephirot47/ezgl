#include "Material.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "TextureFactory.h"

namespace egl
{
void Material::SetDiffuseColor(const Color4f& inDiffuseColor) { mDiffuseColor = inDiffuseColor; }
Color4f Material::GetDiffuseColor() const { return mDiffuseColor; }

void Material::SetSpecularIntensity(const float inSpecularIntensity) { mSpecularIntensity = inSpecularIntensity; }
float Material::GetSpecularIntensity() const { return mSpecularIntensity; }

void Material::SetSpecularExponent(const float inSpecularExponent) { mSpecularExponent = inSpecularExponent; }
float Material::GetSpecularExponent() const { return mSpecularExponent; }

void Material::SetTexture(const std::shared_ptr<Texture2D>& inTexture) { mTexture = inTexture; }
const std::shared_ptr<Texture2D> Material::GetTexture() const { return mTexture; }

void Material::SetLightingEnabled(const bool inLightingEnabled) { mLightingEnabled = inLightingEnabled; }
bool Material::IsLightingEnabled() const { return mLightingEnabled; }

void Material::Bind(ShaderProgram& ioShaderProgram)
{
  if (mTexture)
    mTexture->BindToTextureUnit(0);
  else
    TextureFactory::GetWhiteTexture()->BindToTextureUnit(0);
  ioShaderProgram.SetUniformSafe("UMaterialTexture", 0);

  ioShaderProgram.SetUniformSafe("UMaterialLightingEnabled", mLightingEnabled);
  ioShaderProgram.SetUniformSafe("UMaterialDiffuseColor", mDiffuseColor);
  if (mLightingEnabled)
  {
    ioShaderProgram.SetUniformSafe("UMaterialSpecularIntensity", mSpecularIntensity);
    ioShaderProgram.SetUniformSafe("UMaterialSpecularExponent", mSpecularExponent);
  }
}
}