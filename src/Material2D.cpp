#include "Material2D.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "TextureFactory.h"

namespace egl
{
void Material2D::SetTexture(const std::shared_ptr<Texture2D>& inTexture) { mTexture = inTexture; }
const std::shared_ptr<Texture2D> Material2D::GetTexture() const { return mTexture; }

void Material2D::Bind(ShaderProgram& ioShaderProgram)
{
  if (mTexture)
    mTexture->BindToTextureUnit(0);
  else
    TextureFactory::GetWhiteTexture()->BindToTextureUnit(0);

  ioShaderProgram.SetUniformSafe("UMaterialTexture", 0);
  ioShaderProgram.SetUniformSafe("UMaterialColor", mColor);
}
}