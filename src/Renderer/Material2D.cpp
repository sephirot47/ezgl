#include "ezgl/Material2D.h"
#include "ezgl/ShaderProgram.h"
#include "ezgl/Texture2D.h"
#include "ezgl/TextureFactory.h"

namespace egl
{
void Material2D::SetTexture(const std::shared_ptr<Texture2D>& inTexture) { mTexture = inTexture; }
const std::shared_ptr<Texture2D> Material2D::GetTexture() const { return mTexture; }

void Material2D::Bind(ShaderProgram& ioShaderProgram)
{
  if (mTexture)
    mTexture->BindToTextureUnit(0);
  else
    TextureFactory::GetOneTexture()->BindToTextureUnit(0);

  ioShaderProgram.SetUniformSafe("UMaterialTexture", 0);
  ioShaderProgram.SetUniformSafe("UMaterialColor", mColor);
}
}