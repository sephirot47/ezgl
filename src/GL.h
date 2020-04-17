#pragma once

#include "Color.h"
#include "Flags.h"
#include "Macros.h"
#include "Mat.h"
#include "Span.h"
#include "Vec.h"
#include <GL/glew.h>
#include <array>
#include <cstdint>
#include <string>

namespace egl
{

class GL
{
public:
  using Char = GLchar;
  using Byte = GLbyte;
  using Uint = GLuint;
  using Int = GLint;
  using Id = GLuint;
  using Size = GLsizei;
  using Enum = GLenum;

  static constexpr auto InvalidId = static_cast<GL::Id>(-1);

  enum class EShaderType
  {
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER
  };

  enum class EError
  {
    NO_ERROR = GL_NO_ERROR,
    INVALID_ENUM = GL_INVALID_ENUM,
    INVALID_VALUE = GL_INVALID_VALUE,
    INVALID_OPERATION = GL_INVALID_OPERATION,
    INVALID_FRAMEBUFFER_OPERATION = GL_INVALID_FRAMEBUFFER_OPERATION,
    OUT_OF_MEMORY = GL_OUT_OF_MEMORY,
    STACK_UNDERFLOW = GL_STACK_UNDERFLOW,
    STACK_OVERFLOW = GL_STACK_OVERFLOW
  };

  enum class EBufferType
  {
    ARRAY_BUFFER = GL_ARRAY_BUFFER,
    ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER,
    VERTEX_ARRAY = GL_VERTEX_ARRAY
  };

  enum class EBufferBindingType
  {
    ARRAY_BUFFER = GL_ARRAY_BUFFER_BINDING,
    ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER_BINDING,
    VERTEX_ARRAY = GL_VERTEX_ARRAY_BINDING
  };

  enum class EAccessHint
  {
    STATIC_DRAW = GL_STATIC_DRAW,
    DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
    STREAM_DRAW = GL_STREAM_DRAW,
    STATIC_READ = GL_STATIC_READ,
    DYNAMIC_READ = GL_DYNAMIC_READ,
    STREAM_READ = GL_STREAM_READ,
    STATIC_COPY = GL_STATIC_COPY,
    DYNAMIC_COPY = GL_DYNAMIC_COPY,
    STREAM_COPY = GL_STREAM_COPY
  };

  enum class EBufferBitFlags
  {
    COLOR = GL_COLOR_BUFFER_BIT,
    DEPTH = GL_DEPTH_BUFFER_BIT,
    ACCUM = GL_ACCUM_BUFFER_BIT,
    STENCIL = GL_STENCIL_BUFFER_BIT
  };

  enum class EDataType
  {
    BOOL = GL_BOOL,
    BYTE = GL_BYTE,
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    SHORT = GL_SHORT,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    INT = GL_INT,
    UNSIGNED_INT = GL_UNSIGNED_INT,
    FLOAT = GL_FLOAT,
    DOUBLE = GL_DOUBLE,

    BOOL_VEC2 = GL_BOOL_VEC2,
    BOOL_VEC3 = GL_BOOL_VEC3,
    BOOL_VEC4 = GL_BOOL_VEC4,
    INT_VEC2 = GL_INT_VEC2,
    INT_VEC3 = GL_INT_VEC3,
    INT_VEC4 = GL_INT_VEC4,
    UNSIGNED_INT_VEC2 = GL_UNSIGNED_INT_VEC2,
    UNSIGNED_INT_VEC3 = GL_UNSIGNED_INT_VEC3,
    UNSIGNED_INT_VEC4 = GL_UNSIGNED_INT_VEC4,
    FLOAT_VEC2 = GL_FLOAT_VEC2,
    FLOAT_VEC3 = GL_FLOAT_VEC3,
    FLOAT_VEC4 = GL_FLOAT_VEC4,
    DOUBLE_VEC2 = GL_DOUBLE_VEC2,
    DOUBLE_VEC3 = GL_DOUBLE_VEC3,
    DOUBLE_VEC4 = GL_DOUBLE_VEC4,

    FLOAT_MAT2 = GL_FLOAT_MAT2,
    DOUBLE_MAT2 = GL_DOUBLE_MAT2,
    FLOAT_MAT3 = GL_FLOAT_MAT3,
    DOUBLE_MAT3 = GL_DOUBLE_MAT3,
    FLOAT_MAT4 = GL_FLOAT_MAT4,
    DOUBLE_MAT4 = GL_DOUBLE_MAT4,
  };

  enum class EPrimitivesMode
  {
    TRIANGLES = GL_TRIANGLES,
    LINES = GL_LINES,
    LINES_LOOP = GL_LINE_LOOP,
    LINE_STRIP = GL_LINE_STRIP,
    POINTS = GL_POINTS,
  };

  enum class EShaderInfo
  {
    COMPILE_STATUS = GL_COMPILE_STATUS,
    INFO_LOG_LENGTH = GL_INFO_LOG_LENGTH
  };

  enum class Enablable
  {
    DEPTH_TEST = GL_DEPTH_TEST,
    CULL_FACE = GL_CULL_FACE,
    BLEND = GL_BLEND,
    LINE_SMOOTH = GL_LINE_SMOOTH,
  };

  enum class EShaderProgramEnum
  {
    CURRENT_PROGRAM = GL_CURRENT_PROGRAM
  };

  enum class EBlendFactor
  {
    ZERO = GL_ZERO,
    ONE = GL_ONE,
    SRC_COLOR = GL_SRC_COLOR,
    ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
    DST_COLOR = GL_DST_COLOR,
    ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
    SRC_ALPHA = GL_SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
    DST_ALPHA = GL_DST_ALPHA,
    ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
    CONSTANT_COLOR = GL_CONSTANT_COLOR,
    ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
    CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
    ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
    SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE,
    SRC1_COLOR = GL_SRC1_COLOR,
    ONE_MINUS_SRC1_COLOR = GL_ONE_MINUS_SRC1_COLOR,
    SRC1_ALPHA = GL_SRC1_ALPHA,
    ONE_MINUS_SRC1_ALPHA = GL_ONE_MINUS_SRC1_ALPHA,
  };

  enum class ETextureInternalFormat
  {
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    DEPTH_STENCIL = GL_DEPTH_STENCIL,
    RED = GL_RED,
    RG = GL_RG,
    RGB = GL_RGB,
    RGBA = GL_RGBA,
    R8 = GL_R8,
    R8_SNORM = GL_R8_SNORM,
    R16 = GL_R16,
    R16_SNORM = GL_R16_SNORM,
    RG8 = GL_RG8,
    RG8_SNORM = GL_RG8_SNORM,
    RG16 = GL_RG16,
    RG16_SNORM = GL_RG16_SNORM,
    R3_G3_B2 = GL_R3_G3_B2,
    RGB4 = GL_RGB4,
    RGB5 = GL_RGB5,
    RGB8 = GL_RGB8,
    RGB8_SNORM = GL_RGB8_SNORM,
    RGB10 = GL_RGB10,
    RGB12 = GL_RGB12,
    RGB16_SNORM = GL_RGB16_SNORM,
    RGBA2 = GL_RGBA2,
    RGBA4 = GL_RGBA4,
    RGB5_A1 = GL_RGB5_A1,
    RGBA8 = GL_RGBA8,
    RGBA8_SNORM = GL_RGBA8_SNORM,
    RGB10_A2 = GL_RGB10_A2,
    RGB10_A2UI = GL_RGB10_A2UI,
    RGBA12 = GL_RGBA12,
    RGBA16 = GL_RGBA16,
    SRGB8 = GL_SRGB8,
    SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
    R16F = GL_R16F,
    RG16F = GL_RG16F,
    RGB16F = GL_RGB16F,
    RGBA16F = GL_RGBA16F,
    R32F = GL_R32F,
    RG32F = GL_RG32F,
    RGB32F = GL_RGB32F,
    RGBA32F = GL_RGBA32F,
    R11F_G11F_B10F = GL_R11F_G11F_B10F,
    RGB9_E5 = GL_RGB9_E5,
    R8I = GL_R8I,
    R8UI = GL_R8UI,
    R16I = GL_R16I,
    R16UI = GL_R16UI,
    R32I = GL_R32I,
    R32UI = GL_R32UI,
    RG8I = GL_RG8I,
    RG8UI = GL_RG8UI,
    RG16I = GL_RG16I,
    RG16UI = GL_RG16UI,
    RG32I = GL_RG32I,
    RG32UI = GL_RG32UI,
    RGB8I = GL_RGB8I,
    RGB8UI = GL_RGB8UI,
    RGB16I = GL_RGB16I,
    RGB16UI = GL_RGB16UI,
    RGB32I = GL_RGB32I,
    RGB32UI = GL_RGB32UI,
    RGBA8I = GL_RGBA8I,
    RGBA8UI = GL_RGBA8UI,
    RGBA16I = GL_RGBA16I,
    RGBA16UI = GL_RGBA16UI,
    RGBA32I = GL_RGBA32I,
    RGBA32UI = GL_RGBA32UI,
    COMPRESSED_RED = GL_COMPRESSED_RED,
    COMPRESSED_RG = GL_COMPRESSED_RG,
    COMPRESSED_RGB = GL_COMPRESSED_RGB,
    COMPRESSED_RGBA = GL_COMPRESSED_RGBA,
    COMPRESSED_SRGB = GL_COMPRESSED_SRGB,
    COMPRESSED_SRGB_ALPHA = GL_COMPRESSED_SRGB_ALPHA,
    COMPRESSED_RED_RGTC1 = GL_COMPRESSED_RED_RGTC1,
    COMPRESSED_SIGNED_RED_RGTC1 = GL_COMPRESSED_SIGNED_RED_RGTC1,
    COMPRESSED_RG_RGTC2 = GL_COMPRESSED_RG_RGTC2,
    COMPRESSED_SIGNED_RG_RGTC2 = GL_COMPRESSED_SIGNED_RG_RGTC2,
    COMPRESSED_RGBA_BPTC_UNORM = GL_COMPRESSED_RGBA_BPTC_UNORM,
    COMPRESSED_SRGB_ALPHA_BPTC_UNORM = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
    COMPRESSED_RGB_BPTC_SIGNED_FLOAT = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
    COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,
  };

  enum class ETextureInputFormat
  {
    RED = GL_RED,
    RG = GL_RG,
    RGB = GL_RGB,
    BGR = GL_BGR,
    RGBA = GL_RGBA,
    BGRA = GL_BGRA,
    RED_INTEGER = GL_RED_INTEGER,
    RG_INTEGER = GL_RG_INTEGER,
    RGB_INTEGER = GL_RGB_INTEGER,
    BGR_INTEGER = GL_BGR_INTEGER,
    RGBA_INTEGER = GL_RGBA_INTEGER,
    BGRA_INTEGER = GL_BGRA_INTEGER,
    STENCIL_INDEX = GL_STENCIL_INDEX,
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    DEPTH_STENCIL = GL_DEPTH_STENCIL,
  };

  enum class ETextureInputComponentFormat
  {
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    BYTE = GL_BYTE,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    SHORT = GL_SHORT,
    UNSIGNED_INT = GL_UNSIGNED_INT,
    INT = GL_INT,
    HALF_FLOAT = GL_HALF_FLOAT,
    FLOAT = GL_FLOAT,
    UNSIGNED_BYTE_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
    UNSIGNED_BYTE_2_3_3_REV = GL_UNSIGNED_BYTE_2_3_3_REV,
    UNSIGNED_SHORT_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
    UNSIGNED_SHORT_5_6_5_REV = GL_UNSIGNED_SHORT_5_6_5_REV,
    UNSIGNED_SHORT_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
    UNSIGNED_SHORT_4_4_4_4_REV = GL_UNSIGNED_SHORT_4_4_4_4_REV,
    UNSIGNED_SHORT_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
    UNSIGNED_SHORT_1_5_5_5_REV = GL_UNSIGNED_SHORT_1_5_5_5_REV,
    UNSIGNED_INT_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
    UNSIGNED_INT_8_8_8_8_REV = GL_UNSIGNED_INT_8_8_8_8_REV,
    UNSIGNED_INT_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
    UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
  };

  enum class ETextureTarget
  {
    TEXTURE_1D = GL_TEXTURE_1D,
    TEXTURE_2D = GL_TEXTURE_2D,
    TEXTURE_3D = GL_TEXTURE_3D,
    TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
    TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
    TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
    TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
    TEXTURE_CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
    TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
    TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
    TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
  };

  static void Enable(const GL::Enablable inEnablable);
  static void Disable(const GL::Enablable inEnablable);
  static void SetEnabled(const GL::Enablable inEnablable, const bool inEnabled);

  static void BlendFunc(const GL::EBlendFactor inSourceBlendFactor, const GL::EBlendFactor inDestBlendFactor);

  static void PointSize(const float inPointSize);
  static void LineWidth(const float inLineWidth);

  static GL::Id GenBuffer();
  static GL::Id CreateBuffer();
  static void BindBuffer(const GL::EBufferType inBufferType, const GL::Id inBufferId);
  template <typename T>
  static void BufferData(const GL::Id inBufferId, const Span<T>& inData, const GL::EAccessHint inAccessHint);
  static void DeleteBuffer(const GL::Id inBufferId);

  static GL::Id GenVertexArray();
  static GL::Id CreateVertexArray();
  static void BindVertexArray(const GL::Id inVAOId);
  static void EnableVertexAttribArray(const GL::Id inAttribLocation);
  static void VertexAttribPointer(const GL::Id inAttribLocation,
      const GL::Size inNumComponents,
      const GL::EDataType inDataType,
      const bool inNormalized,
      const GL::Size inStride,
      const GL::Size inOffset = 0);
  static void DisableVertexAttribArray(const GL::Id inAttribLocation);
  static void DeleteVertexArray(const GL::Id inVAOId);

  static GL::Id GenTexture();
  static GL::Id CreateTexture(const GL::ETextureTarget &inTextureTarget);
  static void BindTexture(const GL::ETextureTarget &inTextureTarget, const GL::Id& inTextureId);
  template <typename T>
  static void TexImage2D(const GL::ETextureTarget& inTextureTarget,
      const GL::Size& inWidth,
      const GL::Size& inHeight,
      const GL::ETextureInputFormat& inInputFormat,
      const GL::ETextureInputComponentFormat& inInputComponentFormat,
      const Span<T>& inData,
      const GL::ETextureInternalFormat& inInternalFormat,
      const GL::Int& inMipMapLevel = 0);
  static void GenerateTextureMipMap(const GL::Id& inTextureId);
  static void ActiveTexture(const GL::Id& inTextureUnit);
  static void BindTextureUnit(const GL::Size& inTextureUnit, const GL::Id& inTextureId);
  static void DeleteTexture(const GL::Id& inTextureId);

  static void ClearColor(const Color4f& inColor);
  static void ClearBuffer(const GL::EBufferBitFlags& inBufferBitFlags);
  static void DrawElements(const GL::EPrimitivesMode inPrimitivesMode,
      const GL::Size inNumberOfPrimitives,
      const GL::EDataType inIndicesDataType,
      const GL::Size inBeginPrimiviteIndex = 0);
  static void DrawArrays(const GL::EPrimitivesMode inPrimitivesMode,
      const GL::Size inNumberOfPrimitives,
      const GL::Size inBeginPrimiviteIndex = 0);

  static GL::Id CreateShader(const GL::EShaderType inShaderType);
  static void ShaderSource(const GL::Id inShaderId, const std::string_view inSourceCode);
  static void CompileShader(const GL::Id inShaderId);
  static GL::Int GetShaderInteger(const GL::Id inShaderId, const GL::EShaderInfo inShaderInfoEnum);
  static std::string GetShaderInfoLog(const GL::Id inShaderId);
  static void DeleteShader(const GL::Id inShaderId);

  static GL::Id CreateProgram();
  static void UseProgram(const GL::Id inShaderProgramId);
  static void AttachShader(const GL::Id inShaderProgramId, const GL::Id inShaderId);
  static void LinkProgram(const GL::Id inShaderProgramId);
  static GL::Id GetAttribLocation(const GL::Id inShaderProgramId, const std::string_view inAttribName);
  static GL::Id GetUniformLocation(const GL::Id inShaderProgramId, const std::string_view inUniformName);
  static void Uniform(const GL::Id inUniformLocation, bool inValue);
  static void Uniform(const GL::Id inUniformLocation, int8_t inValue);
  static void Uniform(const GL::Id inUniformLocation, int16_t inValue);
  static void Uniform(const GL::Id inUniformLocation, int32_t inValue);
  static void Uniform(const GL::Id inUniformLocation, float inValue);
  static void Uniform(const GL::Id inUniformLocation, double inValue);
  static void Uniform(const GL::Id inUniformLocation, const Vec2i& inValue);
  static void Uniform(const GL::Id inUniformLocation, const Vec2f& inValue);
  static void Uniform(const GL::Id inUniformLocation, const Vec2d& inValue);
  static void Uniform(const GL::Id inUniformLocation, const Vec3i& inValue);
  static void Uniform(const GL::Id inUniformLocation, const Vec3f& inValue);
  static void Uniform(const GL::Id inUniformLocation, const Vec3d& inValue);
  static void Uniform(const GL::Id inUniformLocation, const Vec4i& inValue);
  static void Uniform(const GL::Id inUniformLocation, const Vec4f& inValue);
  static void Uniform(const GL::Id inUniformLocation, const Vec4d& inValue);
  static void Uniform(const GL::Id inUniformLocation, const Mat2f& inValue);
  static void Uniform(const GL::Id inUniformLocation, const Mat2d& inValue);
  static void Uniform(const GL::Id inUniformLocation, const Mat3f& inValue);
  static void Uniform(const GL::Id inUniformLocation, const Mat3d& inValue);
  static void Uniform(const GL::Id inUniformLocation, const Mat4f& inValue);
  static void Uniform(const GL::Id inUniformLocation, const Mat4d& inValue);
  static void DeleteProgram(const GL::Id inShaderProgramId);

  template <typename T>
  static GL::Int GetInteger(const T& inGLEnum);

  static void GLAPIENTRY ErrorCallback(GL::Enum inSource,
      GL::Enum inType,
      GL::Uint inId,
      GL::Enum inSeverity,
      GL::Size inLength,
      const GL::Char* inMessage,
      const void* inUserParam);

  static GL::EError CheckError();
  static void ClearError();
  static std::string ErrorToString(GL::EError inGLError);

  template <typename T>
  static GL::Enum EnumCast(const T& inGLEnum);

  GL() = delete;
};

// clang-format off
#define GL_SAFE_CALL(EXPR) [&](){ GL::ClearError(); EXPR; const auto error = GL::CheckError(); if (error != GL::EError::NO_ERROR) THROW_EXCEPTION("GL error: " << GL::ErrorToString(error)); }();
#define GL_SAFE_CALL_RET(EXPR) [&](){ GL::ClearError(); const auto ret = EXPR; const auto error = GL::CheckError(); if (error != GL::EError::NO_ERROR) THROW_EXCEPTION("GL error: " << GL::ErrorToString(error)); return ret; }();
// clang-format on

DECLARE_FLAGS(GL::EBufferBitFlags);

}

#include "GL.tcc"