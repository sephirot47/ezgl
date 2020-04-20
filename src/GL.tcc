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
void GL::Bind(const GL::Id inId)
{
  if constexpr (TBindingType == GL::EBindingType::ARRAY_BUFFER)
  {
    GL::BindBuffer(GL::EBufferType::ARRAY_BUFFER, inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::ELEMENT_ARRAY)
  {
    GL::BindBuffer(GL::EBufferType::ELEMENT_ARRAY, inId);
  }
  else if constexpr (TBindingType == GL::EBindingType::FRAMEBUFFER)
  {
    GL::BindFramebuffer(inId);
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
    GL::BindBuffer(GL::EBufferType::VERTEX_ARRAY, inId);
  }
  else
  {
    // Always static_assert (condition is always false). In order for it not to fire always, we have to do this hack,
    // so that the static_assert is dependent on template and not 'static_assert(false, "...")' (this won't work)
    static_assert(std::is_same_v<TBindingType, 99999999>, "Don't know how to bind this buffer type.");
  }
}

template <GL::EBindingType TBindingType>
void GL::UnBind()
{
  GL::Bind<TBindingType>(0);
}

template <GL::EBindingType TBindingType>
GL::BindGuard<TBindingType>::BindGuard() : mPreviouslyBoundId(GL::GetInteger(TBindingType))
{
}

template <GL::EBindingType TBindingType>
GL::BindGuard<TBindingType>::~BindGuard()
{
  GL::Bind<TBindingType>(mPreviouslyBoundId);
}
}