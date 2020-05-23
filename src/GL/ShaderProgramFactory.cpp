#include "ez/ShaderProgramFactory.h"
#include "ez/Shader.h"
#include "ez/ShaderProgram.h"
#include <string_view>

namespace ez
{
std::shared_ptr<ShaderProgram> ShaderProgramFactory::sMeshShaderProgram;
std::shared_ptr<ShaderProgram> ShaderProgramFactory::sTextShaderProgram;
std::shared_ptr<ShaderProgram> ShaderProgramFactory::s2DShaderProgram;
std::shared_ptr<ShaderProgram> ShaderProgramFactory::s2DTextShaderProgram;
std::shared_ptr<ShaderProgram> ShaderProgramFactory::sOnlyColorShaderProgram;
std::shared_ptr<ShaderProgram> ShaderProgramFactory::sDrawFullScreenTextureShaderProgram;

std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string_view inVertexShaderCode,
    const std::string_view inFragmentShaderCode)
{
  return std::make_shared<ShaderProgram>(VertexShader(inVertexShaderCode), FragmentShader(inFragmentShaderCode));
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::GetMeshShaderProgram()
{
  if (!sMeshShaderProgram)
  {
    sMeshShaderProgram = CreateShaderProgram(
#include "Shaders/Mesh.vert"
        ,
#include "Shaders/Mesh.frag"
    );
  }
  return sMeshShaderProgram;
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::GetTextShaderProgram()
{
  if (!sTextShaderProgram)
  {
    sTextShaderProgram = CreateShaderProgram(
#include "Shaders/Text.vert"
        ,
#include "Shaders/Text.frag"
    );
  }
  return sTextShaderProgram;
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::Get2DShaderProgram()
{
  if (!s2DShaderProgram)
  {
    s2DShaderProgram = CreateShaderProgram(
#include "Shaders/2D.vert"
        ,
#include "Shaders/2D.frag"
    );
  }
  return s2DShaderProgram;
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::Get2DTextShaderProgram()
{
  if (!s2DTextShaderProgram)
  {
    s2DTextShaderProgram = CreateShaderProgram(
#include "Shaders/2D.vert"
        ,
#include "Shaders/2DText.frag"
    );
  }
  return s2DTextShaderProgram;
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::GetOnlyColorShaderProgram()
{
  if (!sOnlyColorShaderProgram)
  {
    sOnlyColorShaderProgram = CreateShaderProgram(
#include "Shaders/OnlyColor.vert"
        ,
#include "Shaders/OnlyColor.frag"
    );
  }
  return sOnlyColorShaderProgram;
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::GetDrawFullScreenTextureShaderProgram()
{
  if (!sDrawFullScreenTextureShaderProgram)
  {
    sDrawFullScreenTextureShaderProgram = CreateShaderProgram(
#include "Shaders/DrawFullScreenTexture.vert"
        ,
#include "Shaders/DrawFullScreenTexture.frag"
    );
  }
  return sDrawFullScreenTextureShaderProgram;
}

#undef SHADER_BEGIN
#undef SHADER_END

}