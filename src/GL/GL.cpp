#include "ez/GL.h"

namespace ez
{

void GL::Enable(const GL::EEnablable inEnablable) { glEnable(GL::EnumCast(inEnablable)); }

void GL::Disable(const GL::EEnablable inEnablable) { glDisable(GL::EnumCast(inEnablable)); }

void GL::SetEnabled(const GL::EEnablable inEnablable, const bool inEnabled)
{
  if (inEnabled)
    GL::Enable(inEnablable);
  else
    GL::Disable(inEnablable);
}

bool GL::IsEnabled(const GL::EEnablable inEnablable) { return glIsEnabled(GL::EnumCast(inEnablable)); }

void GL::DepthMask(const bool inDepthMask) { glDepthMask(inDepthMask); }
bool GL::GetDepthMask() { return (GL::GetInteger(GL::EGetEnum::DEPTH_WRITEMASK) == 1); }

void GL::DepthFunc(const GL::EDepthFunc inDepthFunc) { glDepthFunc(GL::EnumCast(inDepthFunc)); }
GL::EDepthFunc GL::GetDepthFunc() { return static_cast<GL::EDepthFunc>(GL::GetInteger(GL::EGetEnum::DEPTH_FUNC)); }

void GL::PointSize(const float inPointSize) { glPointSize(inPointSize); }
float GL::GetPointSize() { return GL::GetFloat(GL::EGetEnum::POINT_SIZE); }

void GL::LineWidth(const float inLineWidth) { glLineWidth(inLineWidth); }
float GL::GetLineWidth() { return GL::GetFloat(GL::EGetEnum::LINE_WIDTH); }

void GL::Viewport(const int inX, const int inY, const int inWidth, const int inHeight)
{
  EXPECTS(inWidth >= 0);
  EXPECTS(inHeight >= 0);
  glViewport(inX, inY, inWidth, inHeight);
}

void GL::Viewport(const Vec2i& inXY, const Vec2i& inSize) { GL::Viewport(inXY[0], inXY[1], inSize[0], inSize[1]); }

void GL::Viewport(const AARecti& inViewport) { GL::Viewport(inViewport.GetMin(), inViewport.GetMax()); }

AARecti GL::GetViewport()
{
  const auto x_y_width_height_ints = GL::GetIntegers<4>(GL::EGetEnum::VIEWPORT);
  const auto x = x_y_width_height_ints[0];
  const auto y = x_y_width_height_ints[1];
  const auto width = x_y_width_height_ints[2];
  const auto height = x_y_width_height_ints[3];
  const auto viewport = AARecti(Vec2i(x, y), Vec2i(width, height));
  return viewport;
}

void GL::BlendFunc(const GL::EBlendFactor inSourceBlendFactor, const GL::EBlendFactor inDestBlendFactor)
{
  glBlendFunc(GL::EnumCast(inSourceBlendFactor), GL::EnumCast(inDestBlendFactor));
}

void GL::BlendFuncSeparate(const GL::EBlendFactor inSourceBlendFactorRGB,
    const GL::EBlendFactor inDestBlendFactorRGB,
    const GL::EBlendFactor inSourceBlendFactorAlpha,
    const GL::EBlendFactor inDestBlendFactorAlpha)

{
  glBlendFuncSeparate(GL::EnumCast(inSourceBlendFactorRGB),
      GL::EnumCast(inDestBlendFactorRGB),
      GL::EnumCast(inSourceBlendFactorAlpha),
      GL::EnumCast(inDestBlendFactorAlpha));
}

void GL::BlendFuncSeparate(const BlendFactors& inBlendFactors)
{
  GL::BlendFuncSeparate(inBlendFactors.mSourceBlendFactorRGB,
      inBlendFactors.mDestBlendFactorRGB,
      inBlendFactors.mSourceBlendFactorAlpha,
      inBlendFactors.mDestBlendFactorAlpha);
}

void GL::BlendColor(const Color4f& inBlendColor)
{
  glBlendColor(inBlendColor[0], inBlendColor[1], inBlendColor[2], inBlendColor[3]);
}

GL::EBlendFactor GL::GetSourceBlendFactorRGB()
{
  return static_cast<GL::EBlendFactor>(GL::GetInteger(GL::EGetEnum::BLEND_SRC_RGB));
}
GL::EBlendFactor GL::GetDestBlendFactorRGB()
{
  return static_cast<GL::EBlendFactor>(GL::GetInteger(GL::EGetEnum::BLEND_DST_RGB));
}
GL::EBlendFactor GL::GetSourceBlendFactorAlpha()
{
  return static_cast<GL::EBlendFactor>(GL::GetInteger(GL::EGetEnum::BLEND_SRC_ALPHA));
}
GL::EBlendFactor GL::GetDestBlendFactorAlpha()
{
  return static_cast<GL::EBlendFactor>(GL::GetInteger(GL::EGetEnum::BLEND_DST_ALPHA));
}
Color4f GL::GetBlendColor()
{
  const auto blend_color_comps = GL::GetFloats<4>(GL::EGetEnum::BLEND_COLOR);
  return Color4f(blend_color_comps[0], blend_color_comps[1], blend_color_comps[2], blend_color_comps[3]);
}

GL::Id GL::GenBuffer()
{
  GL::Id new_buffer_id = 0;
  glGenBuffers(1, &new_buffer_id);
  return new_buffer_id;
}

GL::Id GL::CreateBuffer()
{
  GL::Id new_buffer_id = 0;
  glCreateBuffers(1, &new_buffer_id);
  return new_buffer_id;
}

void GL::BindBuffer(const GL::EBufferType inBufferType, const GL::Id inBufferId)
{
  glBindBuffer(GL::EnumCast(inBufferType), inBufferId);
}

void GL::BindBufferBase(const GL::EBufferType inBufferType, const GL::Id inBindingPoint, const GL::Id inBufferId)
{
  glBindBufferBase(GL::EnumCast(inBufferType), inBindingPoint, inBufferId);
}

GL::EBindingType GL::GetBufferBindingType(const GL::EBufferType inBufferType)
{
  switch (inBufferType)
  {
  case GL::EBufferType::ARRAY_BUFFER:
    return GL::EBindingType::ARRAY_BUFFER;
  case GL::EBufferType::ELEMENT_ARRAY:
    return GL::EBindingType::ELEMENT_ARRAY;
  case GL::EBufferType::UNIFORM_BUFFER:
    return GL::EBindingType::UNIFORM_BUFFER;
  case GL::EBufferType::SHADER_STORAGE_BUFFER:
    return GL::EBindingType::SHADER_STORAGE_BUFFER;
  }
  assert(false);
  return static_cast<GL::EBindingType>(-1);
}

void* GL::MapBuffer(const GL::EBufferType inBufferType, const GL::EAccess inAccess)
{
  return glMapBuffer(GL::EnumCast(inBufferType), GL::EnumCast(inAccess));
}
void* GL::MapBuffer(const GL::Id inBufferId, const GL::EAccess inAccess)
{
  return glMapNamedBuffer(inBufferId, GL::EnumCast(inAccess));
}
void* GL::MapBufferRange(const GL::EBufferType inBufferType,
    const std::size_t inOffset,
    const std::size_t inLength,
    const GL::EMapBufferAccessBitFlags inAccessBitFlags)
{
  return glMapBufferRange(GL::EnumCast(inBufferType), inOffset, inLength, GL::EnumCast(inAccessBitFlags));
}

void* GL::MapBufferRange(const GL::Id inBufferId,
    const std::size_t inOffset,
    const std::size_t inLength,
    const GL::EMapBufferAccessBitFlags inAccessBitFlags)
{
  return glMapNamedBufferRange(inBufferId, inOffset, inLength, GL::EnumCast(inAccessBitFlags));
}

void GL::UnmapBuffer(const GL::EBufferType inBufferType) { glUnmapBuffer(GL::EnumCast(inBufferType)); }
void GL::UnmapBuffer(const GL::Id inBufferId) { glUnmapNamedBuffer(inBufferId); }

void GL::DeleteBuffer(const GL::Id inBufferId) { glDeleteBuffers(1, &inBufferId); }

GL::Id GL::GenVertexArray()
{
  GL::Id new_vertex_array_id = 0;
  glGenVertexArrays(1, &new_vertex_array_id);
  return new_vertex_array_id;
}

GL::Id GL::CreateVertexArray()
{
  GL::Id new_vertex_array_id = 0;
  glCreateVertexArrays(1, &new_vertex_array_id);
  return new_vertex_array_id;
}

void GL::BindVertexArray(const GL::Id inVAOId) { glBindVertexArray(inVAOId); }

void GL::EnableVertexAttribArray(const GL::Id inAttribLocation) { glEnableVertexAttribArray(inAttribLocation); }

void GL::VertexAttribPointer(const GL::Id inAttribLocation,
    const GL::Size inNumComponents,
    const GL::EDataType inDataType,
    const bool inNormalized,
    const GL::Size inStride,
    const GL::Size inOffset)
{
  glVertexAttribPointer(inAttribLocation,
      inNumComponents,
      GL::EnumCast(inDataType),
      inNormalized,
      inStride,
      reinterpret_cast<const void*>(inOffset));
}

void GL::DisableVertexAttribArray(const GL::Id inAttribLocation) { glDisableVertexAttribArray(inAttribLocation); }

void GL::DeleteVertexArray(const GL::Id inVAOId) { glDeleteVertexArrays(1, &inVAOId); }

GL::Id GL::GenTexture()
{
  GL::Id new_texture_id = 0;
  glGenTextures(1, &new_texture_id);
  return new_texture_id;
}

GL::Id GL::CreateTexture(const GL::ETextureTarget& inTextureTarget)
{
  GL::Id new_texture_id = 0;
  glCreateTextures(GL::EnumCast(inTextureTarget), 1, &new_texture_id);
  return new_texture_id;
}

void GL::BindTexture(const GL::ETextureTarget& inTextureTarget, const GL::Id& inTextureId)
{
  glBindTexture(GL::EnumCast(inTextureTarget), inTextureId);
}

void GL::BindImageTexture(const GL::Uint inImageUnit,
    const GL::Id inTextureId,
    const GL::ETextureFormat inFormat,
    const GL::EAccess inAccess,
    const GL::Int inLevel,
    const GL::Boolean inLayered,
    const GL::Int inLayer)
{
  glBindImageTexture(inImageUnit,
      inTextureId,
      inLevel,
      inLayered,
      inLayer,
      GL::EnumCast(inAccess),
      GL::EnumCast(inFormat));
}

void GL::GenerateTextureMipMap(const GL::Id& inTextureId) { glGenerateTextureMipmap(inTextureId); }

void GL::ActiveTexture(const GL::Id& inTextureUnit) { glActiveTexture(inTextureUnit); }
void GL::BindTextureUnit(const GL::Size& inTextureUnit, const GL::Id& inTextureId)
{
  glBindTextureUnit(inTextureUnit, inTextureId);
}

bool GL::IsColorFormat(const GL::ETextureFormat inTextureFormat)
{
  return !IsDepthOnlyFormat(inTextureFormat) && !IsDepthStencilFormat(inTextureFormat);
}

bool GL::IsDepthOnlyFormat(const GL::ETextureFormat inTextureFormat)
{
  return (inTextureFormat == GL::ETextureFormat::DEPTH_COMPONENT
      || inTextureFormat == GL::ETextureFormat::DEPTH_COMPONENT16
      || inTextureFormat == GL::ETextureFormat::DEPTH_COMPONENT24
      || inTextureFormat == GL::ETextureFormat::DEPTH_COMPONENT32F);
}

bool GL::IsDepthStencilFormat(const GL::ETextureFormat inTextureFormat)
{
  return (inTextureFormat == GL::ETextureFormat::DEPTH_STENCIL
      || inTextureFormat == GL::ETextureFormat::DEPTH24_STENCIL8
      || inTextureFormat == GL::ETextureFormat::DEPTH32F_STENCIL8);
}

void GL::DeleteTexture(const GL::Id& inTextureId) { glDeleteTextures(1, &inTextureId); }

GL::Id GL::GenFramebuffer()
{
  GL::Id new_framebuffer_id = 0;
  glGenFramebuffers(1, &new_framebuffer_id);
  return new_framebuffer_id;
}

GL::Id GL::CreateFramebuffer()
{
  GL::Id new_framebuffer_id = 0;
  glGenFramebuffers(1, &new_framebuffer_id);
  return new_framebuffer_id;
}

void GL::BindFramebuffer(const GL::Id inFramebufferId) { glBindFramebuffer(GL_FRAMEBUFFER, inFramebufferId); }

void GL::FramebufferRenderbuffer(const GL::EFramebufferAttachment inAttachment, const GL::Id inRenderbufferId)
{
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL::EnumCast(inAttachment), GL_RENDERBUFFER, inRenderbufferId);
}

void GL::FramebufferTexture2D(const GL::EFramebufferAttachment inAttachment,
    const GL::ETextureTarget inTextureTarget,
    const GL::Id inTextureId,
    const GL::Size inMipmapLevel)
{
  glFramebufferTexture2D(GL_FRAMEBUFFER,
      GL::EnumCast(inAttachment),
      GL::EnumCast(inTextureTarget),
      inTextureId,
      inMipmapLevel);
}

void GL::BlitFramebuffer(const GL::Id inReadFramebufferId,
    const Vec2i& inReadMin,
    const Vec2i& inReadMax,
    const GL::Id inDrawFramebufferId,
    const Vec2i& inDrawMin,
    const Vec2i& inDrawMax,
    const GL::EBufferBitFlags inBufferMask,
    const GL::EFilterMode inFilterType)
{
  glBlitNamedFramebuffer(inReadFramebufferId,
      inDrawFramebufferId,
      inReadMin[0],
      inReadMin[1],
      inReadMax[0],
      inReadMax[1],
      inDrawMin[0],
      inDrawMin[1],
      inDrawMax[0],
      inDrawMax[1],
      GL::EnumCast(inBufferMask),
      GL::EnumCast(inFilterType));
}

void GL::DeleteFramebuffer(const GL::Id inFramebufferId) { glDeleteFramebuffers(1, &inFramebufferId); }

GL::Id GL::GenRenderbuffer()
{
  GL::Id new_renderbuffer_id = 0;
  glGenRenderbuffers(1, &new_renderbuffer_id);
  return new_renderbuffer_id;
}

GL::Id GL::CreateRenderbuffer()
{
  GL::Id new_renderbuffer_id = 0;
  glCreateRenderbuffers(1, &new_renderbuffer_id);
  return new_renderbuffer_id;
}

void GL::RenderbufferStorage(const GL::ETextureFormat inFormat, const GL::Size inWidth, const GL::Size inHeight)
{
  glRenderbufferStorage(GL_RENDERBUFFER, GL::EnumCast(inFormat), inWidth, inHeight);
}

void GL::BindRenderbuffer(const GL::Id inRenderbufferId) { glBindRenderbuffer(GL_RENDERBUFFER, inRenderbufferId); }

void GL::DeleteRenderbuffer(const GL::Id inRenderbufferId) { glDeleteRenderbuffers(1, &inRenderbufferId); }

void GL::ClearColor(const Color4f& inColor)
{
  glClearColor(inColor[0], inColor[1], inColor[2], inColor[3]);
  GL::Clear(GL::EBufferBitFlags::COLOR);
}

void GL::ClearDepth(const float inClearDepth)
{
  glClearDepth(inClearDepth);
  GL::Clear(GL::EBufferBitFlags::DEPTH);
}

void GL::Clear(const GL::EBufferBitFlags& inBufferBitFlags) { glClear(GL::EnumCast(inBufferBitFlags)); }

void GL::DrawElements(const GL::EPrimitivesType inPrimitivesType,
    const GL::Size inNumberOfPrimitives,
    const GL::EDataType inIndicesDataType,
    const GL::Size inBeginPrimiviteIndex)
{
  glDrawElements(GL::EnumCast(inPrimitivesType),
      inNumberOfPrimitives,
      GL::EnumCast(inIndicesDataType),
      reinterpret_cast<const void*>(inBeginPrimiviteIndex));
}

void GL::DrawArrays(const GL::EPrimitivesType inPrimitivesType,
    const GL::Size inNumberOfPrimitives,
    const GL::Size inBeginPrimiviteIndex)
{
  glDrawArrays(GL::EnumCast(inPrimitivesType), inBeginPrimiviteIndex, inNumberOfPrimitives);
}

GL::Id GL::CreateShader(const GL::EShaderType inShaderType)
{
  const auto new_shader_id = glCreateShader(GL::EnumCast(inShaderType));
  return new_shader_id;
}

void GL::ShaderSource(const GL::Id inShaderId, const std::string_view inSourceCode)
{
  const GLchar* source_code_ptr = inSourceCode.data();
  const GLint source_code_length = inSourceCode.size();
  glShaderSource(inShaderId, 1, &source_code_ptr, &source_code_length);
}

void GL::CompileShader(const GL::Id inShaderId) { glCompileShader(inShaderId); }

GL::Int GL::GetShaderInteger(const GL::Id inShaderId, const GL::EShaderInfo inShaderInfoEnum)
{
  GL::Int result = 0;
  glGetShaderiv(inShaderId, GL::EnumCast(inShaderInfoEnum), &result);
  return result;
}

std::string GL::GetShaderInfoLog(const GL::Id inShaderId)
{
  const auto max_error_length = GL::GetShaderInteger(inShaderId, GL::EShaderInfo::INFO_LOG_LENGTH);

  std::string info_msg;
  info_msg.resize(max_error_length);

  GL::Int error_length = 0;
  glGetShaderInfoLog(inShaderId, max_error_length, &error_length, reinterpret_cast<GL::Char*>(info_msg.data()));
  info_msg.resize(error_length - 1);

  return info_msg;
}

void GL::DeleteShader(const GL::Id inShaderId) { glDeleteShader(inShaderId); }

GL::Id GL::CreateProgram() { return glCreateProgram(); }

void GL::UseProgram(const GL::Id inShaderProgramId)
{
  glUseProgram(inShaderProgramId);

#ifndef NDEBUG
  if (inShaderProgramId != 0)
  {
    glValidateProgram(inShaderProgramId);
    int validate_status = 0;
    glGetProgramiv(inShaderProgramId, GL_VALIDATE_STATUS, &validate_status);
    if (validate_status == GL_FALSE)
    {
      int info_length = 0;
      char info_buffer[1024];
      glGetShaderInfoLog(inShaderProgramId, sizeof(info_buffer), &info_length, info_buffer);
      assert(info_length >= 0 && info_length < static_cast<int>(sizeof(info_buffer)));
      info_buffer[info_length] = '\0';

      const auto info = std::string { info_buffer };
      THROW_EXCEPTION("glValidateProgram failed for ShaderProgram with id " << inShaderProgramId << ": " << info);
    }
  }
#endif
}

void GL::AttachShader(const GL::Id inShaderProgramId, const GL::Id inShaderId)
{
  glAttachShader(inShaderProgramId, inShaderId);
}

void GL::LinkProgram(const GL::Id inShaderProgramId) { glLinkProgram(inShaderProgramId); }

void GL::ValidateProgram(const GL::Id inShaderProgramId) { glValidateProgram(inShaderProgramId); }

GL::Id GL::GetAttribLocation(const GL::Id inShaderProgramId, const std::string_view inAttribName)
{
  return glGetAttribLocation(inShaderProgramId, inAttribName.data());
}

GL::Id GL::GetUniformLocation(const GL::Id inShaderProgramId, const std::string_view inUniformName)
{
  return glGetUniformLocation(inShaderProgramId, inUniformName.data());
}

GL::Id GL::GetUniformBlockIndex(const GL::Id inShaderProgramId, const std::string_view inUniformBlockName)
{
  return glGetUniformBlockIndex(inShaderProgramId, inUniformBlockName.data());
}

void GL::DeleteProgram(const GL::Id inShaderProgramId) { glDeleteProgram(inShaderProgramId); }

void GL::Uniform(const GL::Id inUniformLocation, bool inValue)
{
  GL::Uniform(inUniformLocation, static_cast<int32_t>(inValue));
}

void GL::Uniform(const GL::Id inUniformLocation, int8_t inValue)
{
  GL::Uniform(inUniformLocation, static_cast<int32_t>(inValue));
}

void GL::Uniform(const GL::Id inUniformLocation, int16_t inValue)
{
  GL::Uniform(inUniformLocation, static_cast<int32_t>(inValue));
}

void GL::Uniform(const GL::Id inUniformLocation, int32_t inValue) { glUniform1i(inUniformLocation, inValue); }

void GL::Uniform(const GL::Id inUniformLocation, float inValue) { glUniform1f(inUniformLocation, inValue); }

void GL::Uniform(const GL::Id inUniformLocation, double inValue) { glUniform1d(inUniformLocation, inValue); }

void GL::Uniform(const GL::Id inUniformLocation, const Vec2i& inValue)
{
  glUniform2i(inUniformLocation, inValue[0], inValue[1]);
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec2f& inValue)
{
  glUniform2f(inUniformLocation, inValue[0], inValue[1]);
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec2d& inValue)
{
  glUniform2d(inUniformLocation, inValue[0], inValue[1]);
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec3i& inValue)
{
  glUniform3i(inUniformLocation, inValue[0], inValue[1], inValue[2]);
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec3f& inValue)
{
  glUniform3f(inUniformLocation, inValue[0], inValue[1], inValue[2]);
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec3d& inValue)
{
  glUniform3d(inUniformLocation, inValue[0], inValue[1], inValue[2]);
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec4i& inValue)
{
  glUniform4i(inUniformLocation, inValue[0], inValue[1], inValue[2], inValue[3]);
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec4f& inValue)
{
  glUniform4f(inUniformLocation, inValue[0], inValue[1], inValue[2], inValue[3]);
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec4d& inValue)
{
  glUniform4d(inUniformLocation, inValue[0], inValue[1], inValue[2], inValue[3]);
}

void GL::Uniform(const GL::Id inUniformLocation, const Mat2f& inValue)
{
  glUniformMatrix2fv(inUniformLocation, 1, true, inValue.Data());
}

void GL::Uniform(const GL::Id inUniformLocation, const Mat2d& inValue)
{
  glUniformMatrix2dv(inUniformLocation, 1, true, inValue.Data());
}

void GL::Uniform(const GL::Id inUniformLocation, const Mat3f& inValue)
{
  glUniformMatrix3fv(inUniformLocation, 1, true, inValue.Data());
}

void GL::Uniform(const GL::Id inUniformLocation, const Mat3d& inValue)
{
  glUniformMatrix3dv(inUniformLocation, 1, true, inValue.Data());
}

void GL::Uniform(const GL::Id inUniformLocation, const Mat4f& inValue)
{
  glUniformMatrix4fv(inUniformLocation, 1, true, inValue.Data());
}

void GL::Uniform(const GL::Id inUniformLocation, const Mat4d& inValue)
{
  glUniformMatrix4dv(inUniformLocation, 1, true, inValue.Data());
}

void GL::UniformBlockBinding(const GL::Id inShaderProgramId,
    const GL::Id inUniformBlockIndex,
    const GL::Id inBindingPoint)
{
  glUniformBlockBinding(inShaderProgramId, inUniformBlockIndex, inBindingPoint);
}

GL::Sync GL::FenceSync() { return glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0); }

void GL::WaitSync(const GL::Sync inSync, const uint64_t inTimeout) { glWaitSync(inSync, 0, inTimeout); }

GL::EClientWaitSyncResult GL::ClientWaitSync(const GL::Sync inSync, const bool inFlush, const uint64_t inTimeout)
{
  return static_cast<GL::EClientWaitSyncResult>(
      glClientWaitSync(inSync, inFlush ? GL_SYNC_FLUSH_COMMANDS_BIT : 0, inTimeout));
}

void GL::DeleteSync(const GL::Sync inSync) { glDeleteSync(inSync); }

void GL::MemoryBarrier(const GL::EMemoryBarrierBitFlags inMemoryBarrierBitFlags)
{
  glMemoryBarrier(GL::EnumCast(inMemoryBarrierBitFlags));
}

void GL::DispatchCompute(const GL::Uint inNumWorkGroupsX,
    const GL::Uint inNumWorkGroupsY,
    const GL::Uint inNumWorkGroupsZ)
{
  glDispatchCompute(inNumWorkGroupsX, inNumWorkGroupsY, inNumWorkGroupsZ);
}

void GL::TextureParameteri(const GL::Id inTextureId,
    const GL::ETextureParameter inTextureParameter,
    const GL::Int inParameter)
{
  glTextureParameteri(inTextureId, GL::EnumCast(inTextureParameter), inParameter);
}

void GL::SetTextureCompareMode(const GL::Id inTextureId, const GL::ETextureCompareMode inTextureCompareMode)
{
  GL::TextureParameteri(inTextureId, GL::ETextureParameter::TEXTURE_COMPARE_MODE, GL::EnumCast(inTextureCompareMode));
}

GL::Int GL::GetTextureParameteri(const GL::Id inTextureId, const GL::ETextureParameter inTextureParameter)
{
  GL::Int value;
  glGetTextureParameteriv(inTextureId, GL::EnumCast(inTextureParameter), &value);
  return value;
}

void GL::TextureStorage1D(const GL::Id inTextureId,
    const GL::ETextureFormat inTextureFormat,
    const GL::Size inWidth,
    const GL::Size inMipMapLevels)
{
  glTextureStorage1D(inTextureId, inMipMapLevels, GL::EnumCast(inTextureFormat), inWidth);
}

void GL::TextureStorage2D(const GL::Id inTextureId,
    const GL::ETextureFormat inTextureFormat,
    const GL::Size inWidth,
    const GL::Size inHeight,
    const GL::Size inMipMapLevels)
{
  glTextureStorage2D(inTextureId, inMipMapLevels, GL::EnumCast(inTextureFormat), inWidth, inHeight);
}

void GL::TextureStorage3D(const GL::Id inTextureId,
    const GL::ETextureFormat inTextureFormat,
    const GL::Size inWidth,
    const GL::Size inHeight,
    const GL::Size inDepth,
    const GL::Size inMipMapLevels)
{
  glTextureStorage3D(inTextureId, inMipMapLevels, GL::EnumCast(inTextureFormat), inWidth, inHeight, inDepth);
}

bool GL::GetBoolean(const GL::EGetEnum inGetBooleanId)
{
  GL::Boolean result = 0;
  glGetBooleanv(GL::EnumCast(inGetBooleanId), &result);
  return (result != 0);
}

GL::Int GL::GL::GetInteger(const GL::EGetEnum inGetIntegerId)
{
  GL::Int result = 0;
  glGetIntegerv(GL::EnumCast(inGetIntegerId), &result);
  return result;
}

GL::Float GL::GetFloat(const GL::EGetEnum inGetFloatId)
{
  GL::Float result = 0.0f;
  glGetFloatv(GL::EnumCast(inGetFloatId), &result);
  return result;
}

GL::Double GL::GetDouble(const GL::EGetEnum inGetDoubleId)
{
  GL::Double result = 0.0f;
  glGetDoublev(GL::EnumCast(inGetDoubleId), &result);
  return result;
}

void GLAPIENTRY GL::ErrorCallback(GL::Enum inSource,
    GL::Enum inType,
    GL::Uint inId,
    GL::Enum inSeverity,
    GL::Size inLength,
    const GL::Char* inMessage,
    const void* inUserParam)
{
  UNUSED(inSource);
  UNUSED(inId);
  UNUSED(inSeverity);
  UNUSED(inLength);
  UNUSED(inUserParam);

  if (inType == GL_DEBUG_TYPE_ERROR)
  {
    THROW_EXCEPTION("OpenGL error: " << inMessage);
  }
  else
  {
    // std::cout << "OpenGL message: " << inMessage << std::endl;
  }
}

GL::EError GL::CheckError() { return static_cast<GL::EError>(glGetError()); }

void GL::ClearError()
{
  while (GL::CheckError() != GL::EError::NO_ERROR) {}
}

std::string GL::ErrorToString(const GL::EError inGLError)
{
  switch (inGLError)
  {
  case GL::EError::NO_ERROR:
    return "NO_ERROR";
  case GL::EError::INVALID_ENUM:
    return "INVALID_ENUM";
  case GL::EError::INVALID_VALUE:
    return "INVALID_VALUE";
  case GL::EError::INVALID_OPERATION:
    return "INVALID_OPERATION";
  case GL::EError::INVALID_FRAMEBUFFER_OPERATION:
    return "INVALID_FRAMEBUFFER_OPERATION";
  case GL::EError::OUT_OF_MEMORY:
    return "OUT_OF_MEMORY";
  case GL::EError::STACK_UNDERFLOW:
    return "STACK_UNDERFLOW";
  case GL::EError::STACK_OVERFLOW:
    return "STACK_OVERFLOW";
  }
  return "UNKNOWN_ERROR";
}
}