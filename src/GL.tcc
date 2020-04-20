#include "GL.h"

namespace egl
{
template <typename T>
void GL::BufferData(const GL::Id inBufferId, const Span<T>& inData, const GL::EAccessHint inAccessHint)
{
  EXPECTS(inBufferId != 0);
  glNamedBufferData(inBufferId, inData.GetSizeInBytes(), inData.GetData(), GL::EnumCast(inAccessHint));
}

template <typename T>
void GL::BufferSubData(const GL::Id inBufferId, const Span<T>& inData, const GL::Size inOffset)
{
  EXPECTS(inBufferId != 0);
  glNamedBufferSubData(inBufferId, inOffset, inData.GetSizeInBytes(), inData.GetData());
}

template <typename T>
GL::Int GL::GetInteger(const T& inGLEnum)
{
  GL::Int result = 0;
  glGetIntegerv(GL::EnumCast(inGLEnum), &result);
  return result;
}

template <typename T>
GL::Enum GL::EnumCast(const T& inGLEnum)
{
  return static_cast<GL::Enum>(inGLEnum);
}

template <typename T>
void GL::TexImage2D(const GL::ETextureTarget& inTextureTarget,
    const GL::Size& inWidth,
    const GL::Size& inHeight,
    const GL::ETextureInputFormat& inInputFormat,
    const GL::ETextureInputComponentFormat& inInputComponentFormat,
    const Span<T>& inData,
    const GL::ETextureInternalFormat& inInternalFormat,
    const GL::Int& inMipMapLevel)
{
  glTexImage2D(GL::EnumCast(inTextureTarget),
      inMipMapLevel,
      GL::EnumCast(inInternalFormat),
      inWidth,
      inHeight,
      0,
      GL::EnumCast(inInputFormat),
      GL::EnumCast(inInputComponentFormat),
      inData.GetData());
}

template <typename T>
std::vector<T> GL::GetTextureImage(const GL::Id inTextureId,
    const GL::ETextureInputFormat inFormatToConvertTo,
    const GL::EDataType inDataTypeToConvertTo,
    const GL::Size inNumberOfTexelsToRead,
    const GL::Int inMipmapLevel)
{
  std::vector<T> result;
  result.resize(inNumberOfTexelsToRead);

  const auto read_size_in_bytes = result.size() * sizeof(T);
  glGetTextureImage(inTextureId,
      inMipmapLevel,
      GL::EnumCast(inFormatToConvertTo),
      GL::EnumCast(inDataTypeToConvertTo),
      read_size_in_bytes,
      result.data());

  return result;
}

template <GL::EObjectType TObjectType>
GL::Id GL::Create()
{
  if constexpr (TObjectType == GL::EObjectType::EBO || TObjectType == GL::EObjectType::UBO
      || TObjectType == GL::EObjectType::VBO)
  {
    return GL::CreateBuffer();
  }
  else if constexpr (TObjectType == GL::EObjectType::FRAGMENT_SHADER)
  {
    return GL::CreateShader(GL::EShaderType::FRAGMENT);
  }
  else if constexpr (TObjectType == GL::EObjectType::FRAMEBUFFER)
  {
    return GL::CreateFramebuffer();
  }
  else if constexpr (TObjectType == GL::EObjectType::SHADER_PROGRAM)
  {
    return GL::CreateProgram();
  }
  else if constexpr (TObjectType == GL::EObjectType::TEXTURE_2D)
  {
    return GL::CreateTexture(GL::ETextureTarget::TEXTURE_2D);
  }
  else if constexpr (TObjectType == GL::EObjectType::VAO)
  {
    return GL::CreateVertexArray();
  }
  else if constexpr (TObjectType == GL::EObjectType::VERTEX_SHADER)
  {
    return GL::CreateShader(GL::EShaderType::VERTEX);
  }
  else
  {
    static_assert(std::is_same_v<TObjectType, 99999999>, "Don't know how to bind this buffer type.");
  }
  return GL::InvalidId;
}

template <GL::EObjectType TObjectType>
void GL::Delete(const GL::Id inId)
{
  if constexpr (TObjectType == GL::EObjectType::EBO || TObjectType == GL::EObjectType::UBO
      || TObjectType == GL::EObjectType::VBO)
  {
    GL::DeleteBuffer(inId);
  }
  else if constexpr (TObjectType == GL::EObjectType::FRAMEBUFFER)
  {
    GL::DeleteFramebuffer(inId);
  }
  else if constexpr (TObjectType == GL::EObjectType::SHADER_PROGRAM)
  {
    GL::DeleteProgram(inId);
  }
  else if constexpr (TObjectType == GL::EObjectType::TEXTURE_2D)
  {
    GL::DeleteTexture(inId);
  }
  else if constexpr (TObjectType == GL::EObjectType::VAO)
  {
    GL::DeleteVertexArray(inId);
  }
  else if constexpr (TObjectType == GL::EObjectType::FRAGMENT_SHADER || TObjectType == GL::EObjectType::VERTEX_SHADER)
  {
    GL::DeleteShader(inId);
  }
  else
  {
    static_assert(std::is_same_v<TObjectType, 99999999>, "Don't know how to bind this buffer type.");
  }
}

template <GL::EBindingType TBindingType>
void GL::Bind(const GL::Id inId)
{
  if constexpr (TBindingType == GL::EBindingType::ARRAY_BUFFER)
  {
    GL::BindBuffer(GL::EBufferType::ARRAY_BUFFER, inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::CURRENT_PROGRAM)
  {
    GL::UseProgram(inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::ELEMENT_ARRAY)
  {
    GL::BindBuffer(GL::EBufferType::ELEMENT_ARRAY, inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::FRAMEBUFFER)
  {
    GL::BindFramebuffer(inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::RENDERBUFFER)
  {
    GL::BindRenderbuffer(inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_1D)
  {
    GL::BindTexture(GL::ETextureTarget::TEXTURE_1D, inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_1D_ARRAY)
  {
    GL::BindTexture(GL::ETextureTarget::TEXTURE_1D_ARRAY, inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_2D)
  {
    GL::BindTexture(GL::ETextureTarget::TEXTURE_2D, inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_2D_ARRAY)
  {
    GL::BindTexture(GL::ETextureTarget::TEXTURE_2D_ARRAY, inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_3D)
  {
    GL::BindTexture(GL::ETextureTarget::TEXTURE_3D, inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::UNIFORM_BUFFER)
  {
    GL::BindBuffer(GL::EBufferType::UNIFORM_BUFFER, inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::VERTEX_ARRAY)
  {
    GL::BindVertexArray(inId);
  }
  else
  {
    static_assert(std::is_same_v<TBindingType, 99999999>, "Don't know how to bind this buffer type.");
  }
}

template <GL::EBindingType TBindingType>
void GL::UnBind()
{
  GL::Bind<TBindingType>(0);
}

template <GL::EBindingType TBindingType>
GL::BindGuard<TBindingType>::BindGuard() : mPreviouslyBoundId(GL::GetBoundGLId(TBindingType))
{
}

template <GL::EBindingType TBindingType>
GL::BindGuard<TBindingType>::~BindGuard()
{
  GL::Bind<TBindingType>(mPreviouslyBoundId);
}

template <GL::EBindingType TBindingType>
constexpr GL::EObjectType GL::GetObjectType()
{
  if constexpr (TBindingType == GL::EBindingType::VBO)
  {
    return GL::EObjectType::VBO;
  }
  else if constexpr (TBindingType == GL::EBindingType::CURRENT_PROGRAM)
  {
    return GL::EObjectType::SHADER_PROGRAM;
  }
  else if constexpr (TBindingType == GL::EBindingType::EBO)
  {
    return GL::EObjectType::EBO;
  }
  else if constexpr (TBindingType == GL::EBindingType::FRAMEBUFFER)
  {
    return GL::EObjectType::FRAMEBUFFER;
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_2D)
  {
    return GL::EObjectType::TEXTURE_2D;
  }
  else if constexpr (TBindingType == GL::EBindingType::UBO)
  {
    return GL::EObjectType::UBO;
  }
  else if constexpr (TBindingType == GL::EBindingType::VAO)
  {
    return GL::EObjectType::VAO;
  }
  else
  {
    static_assert(std::is_same_v<TBindingType, 99999999>, "Don't know how to bind this buffer type.");
  }
  return GL::EObjectType::FRAGMENT_SHADER;
}

// clang-format off
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EShaderType::VERTEX>() { return GL::EObjectType::VERTEX_SHADER; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EShaderType::FRAGMENT>() { return GL::EObjectType::FRAGMENT_SHADER; }
// clang-format on

}