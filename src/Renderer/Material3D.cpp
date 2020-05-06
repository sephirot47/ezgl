#include "ezgl/Material3D.h"
#include "ezgl/ShaderProgram.h"
#include "ezgl/Texture2D.h"
#include "ezgl/TextureFactory.h"

namespace ez
{
void Material3D::SetDiffuseColor(const Color4f& inDiffuseColor) { mDiffuseColor = inDiffuseColor; }
Color4f Material3D::GetDiffuseColor() const { return mDiffuseColor; }

void Material3D::SetSpecularIntensity(const float inSpecularIntensity) { mSpecularIntensity = inSpecularIntensity; }
float Material3D::GetSpecularIntensity() const { return mSpecularIntensity; }

void Material3D::SetSpecularExponent(const float inSpecularExponent) { mSpecularExponent = inSpecularExponent; }
float Material3D::GetSpecularExponent() const { return mSpecularExponent; }

void Material3D::SetTexture(const std::shared_ptr<Texture2D>& inTexture) { mTexture = inTexture; }
const std::shared_ptr<Texture2D> Material3D::GetTexture() const { return mTexture; }

void Material3D::SetLightingEnabled(const bool inLightingEnabled) { mLightingEnabled = inLightingEnabled; }
bool Material3D::IsLightingEnabled() const { return mLightingEnabled; }

void Material3D::Bind(ShaderProgram& ioShaderProgram)
{
  if (mTexture)
    mTexture->BindToTextureUnit(0);
  else
    TextureFactory::GetOneTexture()->BindToTextureUnit(0);

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