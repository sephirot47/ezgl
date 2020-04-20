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

template <GL::EBindingType TBindingType>
GL::Id GL::Create()
{
  if constexpr (TBindingType == GL::EBindingType::ARRAY_BUFFER)
  {
    return GL::CreateBuffer();
  }
  else if constexpr (TBindingType == GL::EBindingType::CURRENT_PROGRAM)
  {
    return GL::CreateProgram();
  }
  else if constexpr (TBindingType == GL::EBindingType::ELEMENT_ARRAY)
  {
    return GL::CreateBuffer();
  }
  else if constexpr (TBindingType == GL::EBindingType::FRAMEBUFFER)
  {
    return GL::CreateFramebuffer();
  }
  else if constexpr (TBindingType == GL::EBindingType::RENDERBUFFER)
  {
    return GL::CreateRenderbuffer();
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_1D)
  {
    return GL::CreateTexture(GL::ETextureTarget::TEXTURE_1D);
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_1D_ARRAY)
  {
    return GL::CreateTexture(GL::ETextureTarget::TEXTURE_1D_ARRAY);
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_2D)
  {
    return GL::CreateTexture(GL::ETextureTarget::TEXTURE_2D);
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_2D_ARRAY)
  {
    return GL::CreateTexture(GL::ETextureTarget::TEXTURE_2D_ARRAY);
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_3D)
  {
    return GL::CreateTexture(GL::ETextureTarget::TEXTURE_3D);
  }
  else if constexpr (TBindingType == GL::EBindingType::UNIFORM_BUFFER)
  {
    return GL::CreateBuffer();
  }
  else if constexpr (TBindingType == GL::EBindingType::VERTEX_ARRAY)
  {
    return GL::CreateVertexArray();
  }
  else
  {
    static_assert(std::is_same_v<TBindingType, 99999999>, "Don't know how to bind this buffer type.");
  }
  return GL::InvalidId;
}

template <GL::EBindingType TBindingType>
void GL::Delete(const GL::Id inId)
{
  if constexpr (TBindingType == GL::EBindingType::ARRAY_BUFFER)
  {
    GL::DeleteBuffer(inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::CURRENT_PROGRAM)
  {
    GL::DeleteProgram(inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::ELEMENT_ARRAY)
  {
    GL::DeleteBuffer(inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::FRAMEBUFFER)
  {
    GL::DeleteFramebuffer(inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::RENDERBUFFER)
  {
    GL::DeleteRenderbuffer(inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_1D)
  {
    GL::DeleteTexture(inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_1D_ARRAY)
  {
    GL::DeleteTexture(inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_2D)
  {
    GL::DeleteTexture(inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_2D_ARRAY)
  {
    GL::DeleteTexture(inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::TEXTURE_3D)
  {
    GL::DeleteTexture(inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::UNIFORM_BUFFER)
  {
    GL::DeleteBuffer(inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::VERTEX_ARRAY)
  {
    GL::DeleteVertexArray(inId);
  }
  else
  {
    static_assert(std::is_same_v<TBindingType, 99999999>, "Don't know how to bind this buffer type.");
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
}