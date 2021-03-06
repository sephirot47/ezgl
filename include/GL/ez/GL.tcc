#include <ez/GL.h>

namespace ez
{
template <typename T>
void GL::BufferData(const GL::Id inBufferId, const Span<T>& inData, const GL::EBufferDataAccessHint inAccessHint)
{
  EXPECTS(inBufferId != 0);
  glNamedBufferData(inBufferId, inData.GetSizeInBytes(), inData.GetData(), GL::EnumCast(inAccessHint));
}

template <typename T>
void GL::BufferStorage(const GL::Id inBufferId,
    const Span<T>& inData,
    const GL::EBufferStorageAccessHintBitFlags inAccessHint)
{
  EXPECTS(inBufferId != 0);
  glNamedBufferStorage(inBufferId, inData.GetSizeInBytes(), inData.GetData(), GL::EnumCast(inAccessHint));
}

template <typename T>
void GL::BufferSubData(const GL::Id inBufferId, const Span<T>& inData, const GL::Size inOffset)
{
  EXPECTS(inBufferId != 0);
  glNamedBufferSubData(inBufferId, inOffset, inData.GetSizeInBytes(), inData.GetData());
}

template <typename T>
GL::Enum GL::EnumCast(const T& inGLEnum)
{
  return static_cast<GL::Enum>(inGLEnum);
}

template <typename T>
void GL::TexImage1D(const GL::ETextureTarget& inTextureTarget,
    const GL::Size& inWidth,
    const GL::ETextureInputFormat& inInputFormat,
    const GL::ETextureInputComponentFormat& inInputComponentFormat,
    const Span<T>& inData,
    const GL::ETextureFormat& inFormat,
    const GL::Int& inMipMapLevel)
{
  glTexImage1D(GL::EnumCast(inTextureTarget),
      inMipMapLevel,
      GL::EnumCast(inFormat),
      inWidth,
      0,
      GL::EnumCast(inInputFormat),
      GL::EnumCast(inInputComponentFormat),
      inData.GetData());
}

template <typename T>
void GL::TexImage2D(const GL::ETextureTarget& inTextureTarget,
    const GL::Size& inWidth,
    const GL::Size& inHeight,
    const GL::ETextureInputFormat& inInputFormat,
    const GL::ETextureInputComponentFormat& inInputComponentFormat,
    const Span<T>& inData,
    const GL::ETextureFormat& inFormat,
    const GL::Int& inMipMapLevel)
{
  glTexImage2D(GL::EnumCast(inTextureTarget),
      inMipMapLevel,
      GL::EnumCast(inFormat),
      inWidth,
      inHeight,
      0,
      GL::EnumCast(inInputFormat),
      GL::EnumCast(inInputComponentFormat),
      inData.GetData());
}

template <typename T>
void GL::TexImage3D(const GL::ETextureTarget& inTextureTarget,
    const GL::Size& inWidth,
    const GL::Size& inHeight,
    const GL::Size& inDepth,
    const GL::ETextureInputFormat& inInputFormat,
    const GL::ETextureInputComponentFormat& inInputComponentFormat,
    const Span<T>& inData,
    const GL::ETextureFormat& inFormat,
    const GL::Int& inMipMapLevel)
{
  glTexImage3D(GL::EnumCast(inTextureTarget),
      inMipMapLevel,
      GL::EnumCast(inFormat),
      inWidth,
      inHeight,
      inDepth,
      0,
      GL::EnumCast(inInputFormat),
      GL::EnumCast(inInputComponentFormat),
      inData.GetData());
}

template <typename T>
void GL::TextureSubImage1D(const GL::Id inTextureId,
    const GL::Int inXOffset,
    const GL::Size inWidth,
    const GL::ETextureInputFormat inInputFormat,
    const GL::EDataType inInputDataType,
    const Span<T>& inData,
    const GL::Int inMipMapLevel)
{
  glTextureSubImage1D(inTextureId,
      inMipMapLevel,
      inXOffset,
      inWidth,
      GL::EnumCast(inInputFormat),
      GL::EnumCast(inInputDataType),
      inData.GetData());
}

template <typename T>
void GL::TextureSubImage2D(const GL::Id inTextureId,
    const GL::Int inXOffset,
    const GL::Int inYOffset,
    const GL::Size inWidth,
    const GL::Size inHeight,
    const GL::ETextureInputFormat inInputFormat,
    const GL::EDataType inInputDataType,
    const Span<T>& inData,
    const GL::Int inMipMapLevel)
{
  glTextureSubImage2D(inTextureId,
      inMipMapLevel,
      inXOffset,
      inYOffset,
      inWidth,
      inHeight,
      GL::EnumCast(inInputFormat),
      GL::EnumCast(inInputDataType),
      inData.GetData());
}

template <typename T>
void GL::TextureSubImage3D(const GL::Id inTextureId,
    const GL::Int inXOffset,
    const GL::Int inYOffset,
    const GL::Int inZOffset,
    const GL::Size inWidth,
    const GL::Size inHeight,
    const GL::Size inDepth,
    const GL::ETextureInputFormat inInputFormat,
    const GL::EDataType inInputDataType,
    const Span<T>& inData,
    const GL::Int inMipMapLevel)
{
  glTextureSubImage3D(inTextureId,
      inMipMapLevel,
      inXOffset,
      inYOffset,
      inZOffset,
      inWidth,
      inHeight,
      inDepth,
      GL::EnumCast(inInputFormat),
      GL::EnumCast(inInputDataType),
      inData.GetData());
}

template <typename T>
void GL::GetTextureSubImage(const GL::Id inTextureId,
    const GL::Int inXOffset,
    const GL::Int inYOffset,
    const GL::Int inZOffset,
    const GL::Size inWidth,
    const GL::Size inHeight,
    const GL::Size inDepth,
    const GL::ETextureOutputFormat inOutputFormat,
    const GL::ETextureOutputComponentFormat inOutputDataType,
    const GL::Int inMipMapLevel,
    MutableSpan<T> outPixels)
{
  glGetTextureSubImage(inTextureId,
      inMipMapLevel,
      inXOffset,
      inYOffset,
      inZOffset,
      inWidth,
      inHeight,
      inDepth,
      GL::EnumCast(inOutputFormat),
      GL::EnumCast(inOutputDataType),
      outPixels.GetSizeInBytes(),
      outPixels.GetData());
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
void GL::UnBind()
{
  GL::Bind<TBindingType>(0);
}

// clang-format off

template <std::size_t N>
std::array<GL::Int, N> GL::GetIntegers(const GL::EGetEnum inGetIntegerId)
{
  std::array<GL::Int, N> ints;
  glGetIntegerv(GL::EnumCast(inGetIntegerId), ints.data());
  return ints;
}

template <std::size_t N>
std::array<GL::Float, N> GL::GetFloats(const GL::EGetEnum inGetFloatId)
{
  std::array<GL::Float, N> floats;
  glGetFloatv(GL::EnumCast(inGetFloatId), floats.data());
  return floats;
}

template <> inline GL::Id GL::Create<GL::EObjectType::COMPUTE_SHADER>() { return GL::CreateShader(GL::EShaderType::COMPUTE); }
template <> inline GL::Id GL::Create<GL::EObjectType::EBO>() { return GL::CreateBuffer(); }
template <> inline GL::Id GL::Create<GL::EObjectType::FRAGMENT_SHADER>() { return GL::CreateShader(GL::EShaderType::FRAGMENT); }
template <> inline GL::Id GL::Create<GL::EObjectType::FRAMEBUFFER>() { return GL::CreateFramebuffer(); }
template <> inline GL::Id GL::Create<GL::EObjectType::RENDERBUFFER>() { return GL::CreateRenderbuffer(); }
template <> inline GL::Id GL::Create<GL::EObjectType::SHADER_PROGRAM>() { return GL::CreateProgram(); }
template <> inline GL::Id GL::Create<GL::EObjectType::TEXTURE_1D>() { return GL::CreateTexture(GL::ETextureTarget::TEXTURE_1D); }
template <> inline GL::Id GL::Create<GL::EObjectType::TEXTURE_1D_ARRAY>() { return GL::CreateTexture(GL::ETextureTarget::TEXTURE_1D_ARRAY); }
template <> inline GL::Id GL::Create<GL::EObjectType::TEXTURE_2D>() { return GL::CreateTexture(GL::ETextureTarget::TEXTURE_2D); }
template <> inline GL::Id GL::Create<GL::EObjectType::TEXTURE_2D_ARRAY>() { return GL::CreateTexture(GL::ETextureTarget::TEXTURE_2D_ARRAY); }
template <> inline GL::Id GL::Create<GL::EObjectType::TEXTURE_3D>() { return GL::CreateTexture(GL::ETextureTarget::TEXTURE_3D); }
template <> inline GL::Id GL::Create<GL::EObjectType::SSBO>() { return GL::CreateBuffer(); }
template <> inline GL::Id GL::Create<GL::EObjectType::UBO>() { return GL::CreateBuffer(); }
template <> inline GL::Id GL::Create<GL::EObjectType::VAO>() { return GL::CreateVertexArray(); }
template <> inline GL::Id GL::Create<GL::EObjectType::VBO>() { return GL::CreateBuffer(); }
template <> inline GL::Id GL::Create<GL::EObjectType::VERTEX_SHADER>() { return GL::CreateShader(GL::EShaderType::VERTEX ); }

template <> inline void GL::Delete<GL::EObjectType::COMPUTE_SHADER>(const GL::Id inId) { GL::DeleteShader(inId); }
template <> inline void GL::Delete<GL::EObjectType::EBO>(const GL::Id inId) { GL::DeleteBuffer(inId); }
template <> inline void GL::Delete<GL::EObjectType::FRAGMENT_SHADER>(const GL::Id inId) { GL::DeleteShader(inId); }
template <> inline void GL::Delete<GL::EObjectType::FRAMEBUFFER>(const GL::Id inId) { GL::DeleteFramebuffer(inId); }
template <> inline void GL::Delete<GL::EObjectType::RENDERBUFFER>(const GL::Id inId) { GL::DeleteRenderbuffer(inId); }
template <> inline void GL::Delete<GL::EObjectType::SHADER_PROGRAM>(const GL::Id inId) { GL::DeleteProgram(inId); }
template <> inline void GL::Delete<GL::EObjectType::TEXTURE_1D>(const GL::Id inId) { GL::DeleteTexture(inId); }
template <> inline void GL::Delete<GL::EObjectType::TEXTURE_1D_ARRAY>(const GL::Id inId) { GL::DeleteTexture(inId); }
template <> inline void GL::Delete<GL::EObjectType::TEXTURE_2D>(const GL::Id inId) { GL::DeleteTexture(inId); }
template <> inline void GL::Delete<GL::EObjectType::TEXTURE_2D_ARRAY>(const GL::Id inId) { GL::DeleteTexture(inId); }
template <> inline void GL::Delete<GL::EObjectType::TEXTURE_3D>(const GL::Id inId) { GL::DeleteTexture(inId); }
template <> inline void GL::Delete<GL::EObjectType::SSBO>(const GL::Id inId) { GL::DeleteBuffer(inId); }
template <> inline void GL::Delete<GL::EObjectType::UBO>(const GL::Id inId) { GL::DeleteBuffer(inId); }
template <> inline void GL::Delete<GL::EObjectType::VAO>(const GL::Id inId) { GL::DeleteVertexArray(inId); }
template <> inline void GL::Delete<GL::EObjectType::VBO>(const GL::Id inId) { GL::DeleteBuffer(inId); }
template <> inline void GL::Delete<GL::EObjectType::VERTEX_SHADER>(const GL::Id inId) { GL::DeleteShader(inId); }

template <> inline void GL::Bind<GL::EBindingType::EBO>(const GL::Id inId) { GL::BindBuffer(GL::EBufferType::EBO, inId); }
template <> inline void GL::Bind<GL::EBindingType::FRAMEBUFFER>(const GL::Id inId) { GL::BindFramebuffer(inId); }
template <> inline void GL::Bind<GL::EBindingType::RENDERBUFFER>(const GL::Id inId) { GL::BindRenderbuffer(inId); }
template <> inline void GL::Bind<GL::EBindingType::SHADER_PROGRAM>(const GL::Id inId) { GL::UseProgram(inId); }
template <> inline void GL::Bind<GL::EBindingType::TEXTURE_1D>(const GL::Id inId) { GL::BindTexture(GL::ETextureTarget::TEXTURE_1D, inId); }
template <> inline void GL::Bind<GL::EBindingType::TEXTURE_1D_ARRAY>(const GL::Id inId) { GL::BindTexture(GL::ETextureTarget::TEXTURE_1D_ARRAY, inId); }
template <> inline void GL::Bind<GL::EBindingType::TEXTURE_2D>(const GL::Id inId) { GL::BindTexture(GL::ETextureTarget::TEXTURE_2D, inId); }
template <> inline void GL::Bind<GL::EBindingType::TEXTURE_2D_ARRAY>(const GL::Id inId) { GL::BindTexture(GL::ETextureTarget::TEXTURE_2D_ARRAY, inId); }
template <> inline void GL::Bind<GL::EBindingType::TEXTURE_3D>(const GL::Id inId) { GL::BindTexture(GL::ETextureTarget::TEXTURE_3D, inId); }
template <> inline void GL::Bind<GL::EBindingType::SSBO>(const GL::Id inId) { GL::BindBuffer(GL::EBufferType::SSBO, inId); }
template <> inline void GL::Bind<GL::EBindingType::UBO>(const GL::Id inId) { GL::BindBuffer(GL::EBufferType::UBO, inId); }
template <> inline void GL::Bind<GL::EBindingType::VAO>(const GL::Id inId) { GL::BindVertexArray(inId); }
template <> inline void GL::Bind<GL::EBindingType::VBO>(const GL::Id inId) { GL::BindBuffer(GL::EBufferType::VBO, inId); }

template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBindingType::EBO>() { return GL::EObjectType::EBO; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBindingType::FRAMEBUFFER>() { return GL::EObjectType::FRAMEBUFFER; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBindingType::SHADER_PROGRAM>() { return GL::EObjectType::SHADER_PROGRAM; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBindingType::TEXTURE_1D>() { return GL::EObjectType::TEXTURE_1D; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBindingType::TEXTURE_1D_ARRAY>() { return GL::EObjectType::TEXTURE_1D_ARRAY; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBindingType::TEXTURE_2D>() { return GL::EObjectType::TEXTURE_2D; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBindingType::TEXTURE_2D_ARRAY>() { return GL::EObjectType::TEXTURE_2D_ARRAY; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBindingType::TEXTURE_3D>() { return GL::EObjectType::TEXTURE_3D; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBindingType::SSBO>() { return GL::EObjectType::SSBO; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBindingType::UBO>() { return GL::EObjectType::UBO; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBindingType::VBO>() { return GL::EObjectType::VBO; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBindingType::VAO>() { return GL::EObjectType::VAO; }

template <> constexpr GL::EObjectType GL::GetObjectType<GL::EShaderType::VERTEX>() { return GL::EObjectType::VERTEX_SHADER; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EShaderType::FRAGMENT>() { return GL::EObjectType::FRAGMENT_SHADER; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EShaderType::COMPUTE>() { return GL::EObjectType::COMPUTE_SHADER; }

template <> constexpr GL::EObjectType GL::GetObjectType<GL::ETextureTarget::TEXTURE_1D>() { return GL::EObjectType::TEXTURE_1D; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::ETextureTarget::TEXTURE_1D_ARRAY>() { return GL::EObjectType::TEXTURE_1D_ARRAY; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::ETextureTarget::TEXTURE_2D>() { return GL::EObjectType::TEXTURE_2D; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::ETextureTarget::TEXTURE_2D_ARRAY>() { return GL::EObjectType::TEXTURE_2D_ARRAY; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::ETextureTarget::TEXTURE_3D>() { return GL::EObjectType::TEXTURE_3D; }

template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBufferType::EBO>() { return GL::EObjectType::EBO; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBufferType::SSBO>() { return GL::EObjectType::SSBO; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBufferType::UBO>() { return GL::EObjectType::UBO; }
template <> constexpr GL::EObjectType GL::GetObjectType<GL::EBufferType::VBO>() { return GL::EObjectType::VBO; }

template <> constexpr GL::EBindingType GL::GetBindingType<GL::EObjectType::EBO>() { return GL::EBindingType::EBO; }
template <> constexpr GL::EBindingType GL::GetBindingType<GL::EObjectType::FRAMEBUFFER>() { return GL::EBindingType::FRAMEBUFFER; }
template <> constexpr GL::EBindingType GL::GetBindingType<GL::EObjectType::RENDERBUFFER>() { return GL::EBindingType::RENDERBUFFER; }
template <> constexpr GL::EBindingType GL::GetBindingType<GL::EObjectType::SHADER_PROGRAM>() { return GL::EBindingType::SHADER_PROGRAM; }
template <> constexpr GL::EBindingType GL::GetBindingType<GL::EObjectType::TEXTURE_1D>() { return GL::EBindingType::TEXTURE_1D; }
template <> constexpr GL::EBindingType GL::GetBindingType<GL::EObjectType::TEXTURE_1D_ARRAY>() { return GL::EBindingType::TEXTURE_1D_ARRAY; }
template <> constexpr GL::EBindingType GL::GetBindingType<GL::EObjectType::TEXTURE_2D>() { return GL::EBindingType::TEXTURE_2D; }
template <> constexpr GL::EBindingType GL::GetBindingType<GL::EObjectType::TEXTURE_2D_ARRAY>() { return GL::EBindingType::TEXTURE_2D_ARRAY; }
template <> constexpr GL::EBindingType GL::GetBindingType<GL::EObjectType::TEXTURE_3D>() { return GL::EBindingType::TEXTURE_3D; }
template <> constexpr GL::EBindingType GL::GetBindingType<GL::EObjectType::SSBO>() { return GL::EBindingType::SSBO; }
template <> constexpr GL::EBindingType GL::GetBindingType<GL::EObjectType::UBO>() { return GL::EBindingType::UBO; }
template <> constexpr GL::EBindingType GL::GetBindingType<GL::EObjectType::VAO>() { return GL::EBindingType::VAO; }
template <> constexpr GL::EBindingType GL::GetBindingType<GL::EObjectType::VBO>() { return GL::EBindingType::VBO; }

template <GL::EShaderType TShaderType> constexpr GL::EBindingType GL::GetBindingType() { return GL::GetBindingType<GL::GetObjectType<TShaderType>()>(); }
template <GL::ETextureTarget TTextureTarget> constexpr GL::EBindingType GL::GetBindingType() { return GL::GetBindingType<GL::GetObjectType<TTextureTarget>()>(); }
template <GL::EBufferType TBufferType> constexpr GL::EBindingType GL::GetBindingType() { return GL::GetBindingType<GL::GetObjectType<TBufferType>()>(); }

// clang-format on

}
