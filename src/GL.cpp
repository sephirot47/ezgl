#include "GL.h"

namespace egl
{

void GL::Enable(const GL::Enablable inEnablable) { glEnable(GL::EnumCast(inEnablable)); }

void GL::Disable(const GL::Enablable inEnablable) { glDisable(GL::EnumCast(inEnablable)); }

void GL::PointSize(const float inPointSize) { glPointSize(inPointSize); }

void GL::LineWidth(const float inLineWidth) { glLineWidth(inLineWidth); }

void GL::Viewport(const int inX, const int inY, const int inWidth, const int inHeight)
{
  EXPECTS(inWidth >= 0);
  EXPECTS(inHeight >= 0);
  glViewport(inX, inY, inWidth, inHeight);
}

void GL::Viewport(const Vec2i& inXY, const Vec2i& inSize) { GL::Viewport(inXY[0], inXY[1], inSize[0], inSize[1]); }

void GL::SetEnabled(const GL::Enablable inEnablable, const bool inEnabled)
{
  if (inEnabled)
    GL::Enable(inEnablable);
  else
    GL::Disable(inEnablable);
}

void GL::BlendFunc(const GL::EBlendFactor inSourceBlendFactor, const GL::EBlendFactor inDestBlendFactor)
{
  glBlendFunc(GL::EnumCast(inSourceBlendFactor), GL::EnumCast(inDestBlendFactor));
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

GL::EBufferBindingType GL::GetBufferBindingType(const GL::EBufferType inBufferType)
{
  switch (inBufferType)
  {
  case GL::EBufferType::ARRAY_BUFFER: return GL::EBufferBindingType::ARRAY_BUFFER;
  case GL::EBufferType::ELEMENT_ARRAY: return GL::EBufferBindingType::ELEMENT_ARRAY;
  case GL::EBufferType::UNIFORM_BUFFER: return GL::EBufferBindingType::UNIFORM_BUFFER;
  case GL::EBufferType::VERTEX_ARRAY: return GL::EBufferBindingType::VERTEX_ARRAY;
  }
  assert(false);
  return static_cast<GL::EBufferBindingType>(-1);
}

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

void GL::GenerateTextureMipMap(const GL::Id& inTextureId) { glGenerateTextureMipmap(inTextureId); }

void GL::ActiveTexture(const GL::Id& inTextureUnit) { glActiveTexture(inTextureUnit); }
void GL::BindTextureUnit(const GL::Size& inTextureUnit, const GL::Id& inTextureId)
{
  glBindTextureUnit(inTextureUnit, inTextureId);
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

void GL::RenderbufferStorage(const GL::ETextureInternalFormat inInternalFormat,
    const GL::Size inWidth,
    const GL::Size inHeight)
{
  glRenderbufferStorage(GL_RENDERBUFFER, GL::EnumCast(inInternalFormat), inWidth, inHeight);
}

void GL::BindRenderbuffer(const GL::Id inRenderbufferId) { glBindRenderbuffer(GL_RENDERBUFFER, inRenderbufferId); }

void GL::DeleteRenderBuffer(const GL::Id inRenderbufferId) { glDeleteRenderbuffers(1, &inRenderbufferId); }

void GL::ClearColor(const Color4f& inColor) { glClearColor(inColor[0], inColor[1], inColor[2], inColor[3]); }
void GL::ClearBuffer(const GL::EBufferBitFlags& inBufferBitFlags) { glClear(GL::EnumCast(inBufferBitFlags)); }

void GL::DrawElements(const GL::EPrimitivesMode inPrimitivesMode,
    const GL::Size inNumberOfPrimitives,
    const GL::EDataType inIndicesDataType,
    const GL::Size inBeginPrimiviteIndex)
{
  glDrawElements(GL::EnumCast(inPrimitivesMode),
      inNumberOfPrimitives,
      GL::EnumCast(inIndicesDataType),
      reinterpret_cast<const void*>(inBeginPrimiviteIndex));
}

void GL::DrawArrays(const GL::EPrimitivesMode inPrimitivesMode,
    const GL::Size inNumberOfPrimitives,
    const GL::Size inBeginPrimiviteIndex)
{
  glDrawArrays(GL::EnumCast(inPrimitivesMode), inBeginPrimiviteIndex, inNumberOfPrimitives);
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

void GL::UseProgram(const GL::Id inShaderProgramId) { glUseProgram(inShaderProgramId); }

void GL::AttachShader(const GL::Id inShaderProgramId, const GL::Id inShaderId)
{
  glAttachShader(inShaderProgramId, inShaderId);
}

void GL::LinkProgram(const GL::Id inShaderProgramId) { glLinkProgram(inShaderProgramId); }

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
  case GL::EError::NO_ERROR: return "NO_ERROR";
  case GL::EError::INVALID_ENUM: return "INVALID_ENUM";
  case GL::EError::INVALID_VALUE: return "INVALID_VALUE";
  case GL::EError::INVALID_OPERATION: return "INVALID_OPERATION";
  case GL::EError::INVALID_FRAMEBUFFER_OPERATION: return "INVALID_FRAMEBUFFER_OPERATION";
  case GL::EError::OUT_OF_MEMORY: return "OUT_OF_MEMORY";
  case GL::EError::STACK_UNDERFLOW: return "STACK_UNDERFLOW";
  case GL::EError::STACK_OVERFLOW: return "STACK_OVERFLOW";
  }
  return "UNKNOWN_ERROR";
}
}