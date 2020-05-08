#pragma once

#include "ez/Color.h"
#include "ez/Flags.h"
#include "ez/Macros.h"
#include "ez/Mat.h"
#include "ez/AARect.h"
#include "ez/Span.h"
#include "ez/Vec.h"
#include <GL/glew.h>
#include <array>
#include <cstdint>
#include <string>

namespace ez
{

class GL final
{
public:
  using Char = GLchar;
  using Byte = GLbyte;
  using Uint = GLuint;
  using Boolean = GLboolean;
  using Int = GLint;
  using Float = GLfloat;
  using Double= GLdouble;
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
    UNIFORM_BUFFER = GL_UNIFORM_BUFFER,

    // Aliases
    EBO = GL_ELEMENT_ARRAY_BUFFER,
    VBO = GL_ARRAY_BUFFER,
    UBO = GL_UNIFORM_BUFFER,
  };

  enum class EObjectType
  {
    EBO,
    FRAMEBUFFER,
    RENDERBUFFER,
    TEXTURE_1D,
    TEXTURE_1D_ARRAY,
    TEXTURE_2D,
    TEXTURE_2D_ARRAY,
    TEXTURE_3D,
    VAO,
    VBO,
    UBO,
    SHADER_PROGRAM,
    FRAGMENT_SHADER,
    VERTEX_SHADER,
  };

  enum class EBindingType
  {
    ARRAY_BUFFER = GL_ARRAY_BUFFER_BINDING,
    CURRENT_PROGRAM = GL_CURRENT_PROGRAM,
    ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER_BINDING,
    FRAMEBUFFER = GL_FRAMEBUFFER_BINDING,
    RENDERBUFFER = GL_RENDERBUFFER_BINDING,
    TEXTURE_1D = GL_TEXTURE_BINDING_1D,
    TEXTURE_1D_ARRAY = GL_TEXTURE_BINDING_1D_ARRAY,
    TEXTURE_2D = GL_TEXTURE_BINDING_2D,
    TEXTURE_2D_ARRAY = GL_TEXTURE_BINDING_2D_ARRAY,
    TEXTURE_3D = GL_TEXTURE_BINDING_3D,
    UNIFORM_BUFFER = GL_UNIFORM_BUFFER_BINDING,
    VERTEX_ARRAY = GL_VERTEX_ARRAY_BINDING,

    // Aliases
    EBO = GL_ELEMENT_ARRAY_BUFFER_BINDING,
    SHADER_PROGRAM = GL_CURRENT_PROGRAM,
    UBO = GL_UNIFORM_BUFFER_BINDING,
    VAO = GL_VERTEX_ARRAY_BINDING,
    VBO = GL_ARRAY_BUFFER_BINDING,
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

  enum class EPrimitivesType
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

  enum class EEnablable
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

  enum class ETextureFormat
  {
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
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
    DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
    DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
    DEPTH_STENCIL = GL_DEPTH_STENCIL,
    DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
    DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
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

  enum class EFramebufferAttachment
  {
    COLOR_ATTACHMENT0 = GL_COLOR_ATTACHMENT0,
    COLOR_ATTACHMENT1 = GL_COLOR_ATTACHMENT1,
    COLOR_ATTACHMENT2 = GL_COLOR_ATTACHMENT2,
    COLOR_ATTACHMENT3 = GL_COLOR_ATTACHMENT3,
    COLOR_ATTACHMENT4 = GL_COLOR_ATTACHMENT4,
    COLOR_ATTACHMENT5 = GL_COLOR_ATTACHMENT5,
    COLOR_ATTACHMENT6 = GL_COLOR_ATTACHMENT6,
    COLOR_ATTACHMENT7 = GL_COLOR_ATTACHMENT7,
    COLOR_ATTACHMENT8 = GL_COLOR_ATTACHMENT8,
    COLOR_ATTACHMENT9 = GL_COLOR_ATTACHMENT9,
    COLOR_ATTACHMENT10 = GL_COLOR_ATTACHMENT10,
    COLOR_ATTACHMENT11 = GL_COLOR_ATTACHMENT11,
    COLOR_ATTACHMENT12 = GL_COLOR_ATTACHMENT12,
    COLOR_ATTACHMENT13 = GL_COLOR_ATTACHMENT13,
    COLOR_ATTACHMENT14 = GL_COLOR_ATTACHMENT14,
    COLOR_ATTACHMENT15 = GL_COLOR_ATTACHMENT15,
    DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
    STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT,
    DEPTH_STENCIL_ATTACHMENT = GL_DEPTH_STENCIL_ATTACHMENT,
  };

  enum class ETextureParameter
  {
    DEPTH_STENCIL_TEXTURE_MODE = GL_DEPTH_STENCIL_TEXTURE_MODE,
    TEXTURE_BASE_LEVEL = GL_TEXTURE_BASE_LEVEL,
    TEXTURE_BORDER_COLOR = GL_TEXTURE_BORDER_COLOR,
    TEXTURE_COMPARE_FUNC = GL_TEXTURE_COMPARE_FUNC,
    TEXTURE_COMPARE_MODE = GL_TEXTURE_COMPARE_MODE,
    TEXTURE_LOD_BIAS = GL_TEXTURE_LOD_BIAS,
    TEXTURE_MIN_FILTER = GL_TEXTURE_MIN_FILTER,
    TEXTURE_MAG_FILTER = GL_TEXTURE_MAG_FILTER,
    TEXTURE_MIN_LOD = GL_TEXTURE_MIN_LOD,
    TEXTURE_MAX_LOD = GL_TEXTURE_MAX_LOD,
    TEXTURE_MAX_LEVEL = GL_TEXTURE_MAX_LEVEL,
    TEXTURE_SWIZZLE_R = GL_TEXTURE_SWIZZLE_R,
    TEXTURE_SWIZZLE_G = GL_TEXTURE_SWIZZLE_G,
    TEXTURE_SWIZZLE_B = GL_TEXTURE_SWIZZLE_B,
    TEXTURE_SWIZZLE_A = GL_TEXTURE_SWIZZLE_A,
    TEXTURE_SWIZZLE_RGBA = GL_TEXTURE_SWIZZLE_RGBA,
    TEXTURE_WRAP_S = GL_TEXTURE_WRAP_S,
    TEXTURE_WRAP_T = GL_TEXTURE_WRAP_T,
    TEXTURE_WRAP_R = GL_TEXTURE_WRAP_R,
  };

  enum class EFilterType
  {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
  };

  enum class EDepthFunc
  {
    NEVER = GL_NEVER,
    LESS = GL_LESS,
    EQUAL = GL_EQUAL,
    LEQUAL = GL_LEQUAL,
    GREATER = GL_GREATER,
    NOTEQUAL = GL_NOTEQUAL,
    GEQUAL = GL_GEQUAL,
    ALWAYS = GL_ALWAYS,
  };

  enum class EGetEnum
  {
    ACTIVE_TEXTURE = GL_ACTIVE_TEXTURE,
    ALIASED_LINE_WIDTH_RANGE = GL_ALIASED_LINE_WIDTH_RANGE,
    ARRAY_BUFFER_BINDING = GL_ARRAY_BUFFER_BINDING,
    BLEND = GL_BLEND,
    BLEND_COLOR = GL_BLEND_COLOR,
    BLEND_DST_ALPHA = GL_BLEND_DST_ALPHA,
    BLEND_DST_RGB = GL_BLEND_DST_RGB,
    BLEND_EQUATION_RGB = GL_BLEND_EQUATION_RGB,
    BLEND_EQUATION_ALPHA = GL_BLEND_EQUATION_ALPHA,
    BLEND_SRC_ALPHA = GL_BLEND_SRC_ALPHA,
    BLEND_SRC_RGB = GL_BLEND_SRC_RGB,
    COLOR_CLEAR_VALUE = GL_COLOR_CLEAR_VALUE,
    COLOR_LOGIC_OP = GL_COLOR_LOGIC_OP,
    COLOR_WRITEMASK = GL_COLOR_WRITEMASK,
    COMPRESSED_TEXTURE_FORMATS = GL_COMPRESSED_TEXTURE_FORMATS,
    MAX_COMPUTE_SHADER_STORAGE_BLOCKS = GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS,
    MAX_COMBINED_SHADER_STORAGE_BLOCKS = GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS,
    MAX_COMPUTE_UNIFORM_BLOCKS = GL_MAX_COMPUTE_UNIFORM_BLOCKS,
    MAX_COMPUTE_TEXTURE_IMAGE_UNITS = GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS,
    MAX_COMPUTE_UNIFORM_COMPONENTS = GL_MAX_COMPUTE_UNIFORM_COMPONENTS,
    MAX_COMPUTE_ATOMIC_COUNTERS = GL_MAX_COMPUTE_ATOMIC_COUNTERS,
    MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS = GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS,
    MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS = GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS,
    MAX_COMPUTE_WORK_GROUP_INVOCATIONS = GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS,
    MAX_COMPUTE_WORK_GROUP_COUNT = GL_MAX_COMPUTE_WORK_GROUP_COUNT,
    MAX_COMPUTE_WORK_GROUP_SIZE = GL_MAX_COMPUTE_WORK_GROUP_SIZE,
    DISPATCH_INDIRECT_BUFFER_BINDING = GL_DISPATCH_INDIRECT_BUFFER_BINDING,
    MAX_DEBUG_GROUP_STACK_DEPTH = GL_MAX_DEBUG_GROUP_STACK_DEPTH,
    DEBUG_GROUP_STACK_DEPTH = GL_DEBUG_GROUP_STACK_DEPTH,
    CONTEXT_FLAGS = GL_CONTEXT_FLAGS,
    CULL_FACE = GL_CULL_FACE,
    CULL_FACE_MODE = GL_CULL_FACE_MODE,
    CURRENT_PROGRAM = GL_CURRENT_PROGRAM,
    DEPTH_CLEAR_VALUE = GL_DEPTH_CLEAR_VALUE,
    DEPTH_FUNC = GL_DEPTH_FUNC,
    DEPTH_RANGE = GL_DEPTH_RANGE,
    DEPTH_TEST = GL_DEPTH_TEST,
    DEPTH_WRITEMASK = GL_DEPTH_WRITEMASK,
    DITHER = GL_DITHER,
    DOUBLEBUFFER = GL_DOUBLEBUFFER,
    DRAW_BUFFER = GL_DRAW_BUFFER,
    DRAW_BUFFER0 = GL_DRAW_BUFFER0,
    DRAW_BUFFER1 = GL_DRAW_BUFFER1,
    DRAW_BUFFER2 = GL_DRAW_BUFFER2,
    DRAW_BUFFER3 = GL_DRAW_BUFFER3,
    DRAW_BUFFER4 = GL_DRAW_BUFFER4,
    DRAW_BUFFER5 = GL_DRAW_BUFFER5,
    DRAW_BUFFER6 = GL_DRAW_BUFFER6,
    DRAW_BUFFER7 = GL_DRAW_BUFFER7,
    DRAW_BUFFER8 = GL_DRAW_BUFFER8,
    DRAW_BUFFER9 = GL_DRAW_BUFFER9,
    DRAW_FRAMEBUFFER_BINDING = GL_DRAW_FRAMEBUFFER_BINDING,
    READ_FRAMEBUFFER_BINDING = GL_READ_FRAMEBUFFER_BINDING,
    ELEMENT_ARRAY_BUFFER_BINDING = GL_ELEMENT_ARRAY_BUFFER_BINDING,
    FRAGMENT_SHADER_DERIVATIVE_HINT = GL_FRAGMENT_SHADER_DERIVATIVE_HINT,
    IMPLEMENTATION_COLOR_READ_FORMAT = GL_IMPLEMENTATION_COLOR_READ_FORMAT,
    IMPLEMENTATION_COLOR_READ_TYPE = GL_IMPLEMENTATION_COLOR_READ_TYPE,
    LINE_SMOOTH = GL_LINE_SMOOTH,
    LINE_SMOOTH_HINT = GL_LINE_SMOOTH_HINT,
    LINE_WIDTH = GL_LINE_WIDTH,
    LAYER_PROVOKING_VERTEX = GL_LAYER_PROVOKING_VERTEX,
    LOGIC_OP_MODE = GL_LOGIC_OP_MODE,
    MAJOR_VERSION = GL_MAJOR_VERSION,
    MAX_3D_TEXTURE_SIZE = GL_MAX_3D_TEXTURE_SIZE,
    MAX_ARRAY_TEXTURE_LAYERS = GL_MAX_ARRAY_TEXTURE_LAYERS,
    MAX_CLIP_DISTANCES = GL_MAX_CLIP_DISTANCES,
    MAX_COLOR_TEXTURE_SAMPLES = GL_MAX_COLOR_TEXTURE_SAMPLES,
    MAX_COMBINED_ATOMIC_COUNTERS = GL_MAX_COMBINED_ATOMIC_COUNTERS,
    MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS = GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS,
    MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS = GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS,
    MAX_COMBINED_TEXTURE_IMAGE_UNITS = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
    MAX_COMBINED_UNIFORM_BLOCKS = GL_MAX_COMBINED_UNIFORM_BLOCKS,
    MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS = GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS,
    MAX_CUBE_MAP_TEXTURE_SIZE = GL_MAX_CUBE_MAP_TEXTURE_SIZE,
    MAX_DEPTH_TEXTURE_SAMPLES = GL_MAX_DEPTH_TEXTURE_SAMPLES,
    MAX_DRAW_BUFFERS = GL_MAX_DRAW_BUFFERS,
    MAX_DUAL_SOURCE_DRAW_BUFFERS = GL_MAX_DUAL_SOURCE_DRAW_BUFFERS,
    MAX_ELEMENTS_INDICES = GL_MAX_ELEMENTS_INDICES,
    MAX_ELEMENTS_VERTICES = GL_MAX_ELEMENTS_VERTICES,
    MAX_FRAGMENT_ATOMIC_COUNTERS = GL_MAX_FRAGMENT_ATOMIC_COUNTERS,
    MAX_FRAGMENT_SHADER_STORAGE_BLOCKS = GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS,
    MAX_FRAGMENT_INPUT_COMPONENTS = GL_MAX_FRAGMENT_INPUT_COMPONENTS,
    MAX_FRAGMENT_UNIFORM_COMPONENTS = GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
    MAX_FRAGMENT_UNIFORM_VECTORS = GL_MAX_FRAGMENT_UNIFORM_VECTORS,
    MAX_FRAGMENT_UNIFORM_BLOCKS = GL_MAX_FRAGMENT_UNIFORM_BLOCKS,
    MAX_FRAMEBUFFER_WIDTH = GL_MAX_FRAMEBUFFER_WIDTH,
    MAX_FRAMEBUFFER_HEIGHT = GL_MAX_FRAMEBUFFER_HEIGHT,
    MAX_FRAMEBUFFER_LAYERS = GL_MAX_FRAMEBUFFER_LAYERS,
    MAX_FRAMEBUFFER_SAMPLES = GL_MAX_FRAMEBUFFER_SAMPLES,
    MAX_GEOMETRY_ATOMIC_COUNTERS = GL_MAX_GEOMETRY_ATOMIC_COUNTERS,
    MAX_GEOMETRY_SHADER_STORAGE_BLOCKS = GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS,
    MAX_GEOMETRY_INPUT_COMPONENTS = GL_MAX_GEOMETRY_INPUT_COMPONENTS,
    MAX_GEOMETRY_OUTPUT_COMPONENTS = GL_MAX_GEOMETRY_OUTPUT_COMPONENTS,
    MAX_GEOMETRY_TEXTURE_IMAGE_UNITS = GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS,
    MAX_GEOMETRY_UNIFORM_BLOCKS = GL_MAX_GEOMETRY_UNIFORM_BLOCKS,
    MAX_GEOMETRY_UNIFORM_COMPONENTS = GL_MAX_GEOMETRY_UNIFORM_COMPONENTS,
    MAX_INTEGER_SAMPLES = GL_MAX_INTEGER_SAMPLES,
    MIN_MAP_BUFFER_ALIGNMENT = GL_MIN_MAP_BUFFER_ALIGNMENT,
    MAX_LABEL_LENGTH = GL_MAX_LABEL_LENGTH,
    MAX_PROGRAM_TEXEL_OFFSET = GL_MAX_PROGRAM_TEXEL_OFFSET,
    MIN_PROGRAM_TEXEL_OFFSET = GL_MIN_PROGRAM_TEXEL_OFFSET,
    MAX_RECTANGLE_TEXTURE_SIZE = GL_MAX_RECTANGLE_TEXTURE_SIZE,
    MAX_RENDERBUFFER_SIZE = GL_MAX_RENDERBUFFER_SIZE,
    MAX_SAMPLE_MASK_WORDS = GL_MAX_SAMPLE_MASK_WORDS,
    MAX_SERVER_WAIT_TIMEOUT = GL_MAX_SERVER_WAIT_TIMEOUT,
    MAX_SHADER_STORAGE_BUFFER_BINDINGS = GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS,
    MAX_TESS_CONTROL_ATOMIC_COUNTERS = GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS,
    MAX_TESS_EVALUATION_ATOMIC_COUNTERS = GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS,
    MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS = GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS,
    MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS = GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS,
    MAX_TEXTURE_BUFFER_SIZE = GL_MAX_TEXTURE_BUFFER_SIZE,
    MAX_TEXTURE_IMAGE_UNITS = GL_MAX_TEXTURE_IMAGE_UNITS,
    MAX_TEXTURE_LOD_BIAS = GL_MAX_TEXTURE_LOD_BIAS,
    MAX_TEXTURE_SIZE = GL_MAX_TEXTURE_SIZE,
    MAX_UNIFORM_BUFFER_BINDINGS = GL_MAX_UNIFORM_BUFFER_BINDINGS,
    MAX_UNIFORM_BLOCK_SIZE = GL_MAX_UNIFORM_BLOCK_SIZE,
    MAX_UNIFORM_LOCATIONS = GL_MAX_UNIFORM_LOCATIONS,
    MAX_VARYING_COMPONENTS = GL_MAX_VARYING_COMPONENTS,
    MAX_VARYING_VECTORS = GL_MAX_VARYING_VECTORS,
    MAX_VARYING_FLOATS = GL_MAX_VARYING_FLOATS,
    MAX_VERTEX_ATOMIC_COUNTERS = GL_MAX_VERTEX_ATOMIC_COUNTERS,
    MAX_VERTEX_ATTRIBS = GL_MAX_VERTEX_ATTRIBS,
    MAX_VERTEX_SHADER_STORAGE_BLOCKS = GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS,
    MAX_VERTEX_TEXTURE_IMAGE_UNITS = GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
    MAX_VERTEX_UNIFORM_COMPONENTS = GL_MAX_VERTEX_UNIFORM_COMPONENTS,
    MAX_VERTEX_UNIFORM_VECTORS = GL_MAX_VERTEX_UNIFORM_VECTORS,
    MAX_VERTEX_OUTPUT_COMPONENTS = GL_MAX_VERTEX_OUTPUT_COMPONENTS,
    MAX_VERTEX_UNIFORM_BLOCKS = GL_MAX_VERTEX_UNIFORM_BLOCKS,
    MAX_VIEWPORT_DIMS = GL_MAX_VIEWPORT_DIMS,
    MAX_VIEWPORTS = GL_MAX_VIEWPORTS,
    MINOR_VERSION = GL_MINOR_VERSION,
    NUM_COMPRESSED_TEXTURE_FORMATS = GL_NUM_COMPRESSED_TEXTURE_FORMATS,
    NUM_EXTENSIONS = GL_NUM_EXTENSIONS,
    NUM_PROGRAM_BINARY_FORMATS = GL_NUM_PROGRAM_BINARY_FORMATS,
    NUM_SHADER_BINARY_FORMATS = GL_NUM_SHADER_BINARY_FORMATS,
    PACK_ALIGNMENT = GL_PACK_ALIGNMENT,
    PACK_IMAGE_HEIGHT = GL_PACK_IMAGE_HEIGHT,
    PACK_LSB_FIRST = GL_PACK_LSB_FIRST,
    PACK_ROW_LENGTH = GL_PACK_ROW_LENGTH,
    PACK_SKIP_IMAGES = GL_PACK_SKIP_IMAGES,
    PACK_SKIP_PIXELS = GL_PACK_SKIP_PIXELS,
    PACK_SKIP_ROWS = GL_PACK_SKIP_ROWS,
    PACK_SWAP_BYTES = GL_PACK_SWAP_BYTES,
    PIXEL_PACK_BUFFER_BINDING = GL_PIXEL_PACK_BUFFER_BINDING,
    PIXEL_UNPACK_BUFFER_BINDING = GL_PIXEL_UNPACK_BUFFER_BINDING,
    POINT_FADE_THRESHOLD_SIZE = GL_POINT_FADE_THRESHOLD_SIZE,
    PRIMITIVE_RESTART_INDEX = GL_PRIMITIVE_RESTART_INDEX,
    PROGRAM_BINARY_FORMATS = GL_PROGRAM_BINARY_FORMATS,
    PROGRAM_PIPELINE_BINDING = GL_PROGRAM_PIPELINE_BINDING,
    PROGRAM_POINT_SIZE = GL_PROGRAM_POINT_SIZE,
    PROVOKING_VERTEX = GL_PROVOKING_VERTEX,
    POINT_SIZE = GL_POINT_SIZE,
    POINT_SIZE_GRANULARITY = GL_POINT_SIZE_GRANULARITY,
    POINT_SIZE_RANGE = GL_POINT_SIZE_RANGE,
    POLYGON_OFFSET_FACTOR = GL_POLYGON_OFFSET_FACTOR,
    POLYGON_OFFSET_UNITS = GL_POLYGON_OFFSET_UNITS,
    POLYGON_OFFSET_FILL = GL_POLYGON_OFFSET_FILL,
    POLYGON_OFFSET_LINE = GL_POLYGON_OFFSET_LINE,
    POLYGON_OFFSET_POINT = GL_POLYGON_OFFSET_POINT,
    POLYGON_SMOOTH = GL_POLYGON_SMOOTH,
    POLYGON_SMOOTH_HINT = GL_POLYGON_SMOOTH_HINT,
    READ_BUFFER = GL_READ_BUFFER,
    RENDERBUFFER_BINDING = GL_RENDERBUFFER_BINDING,
    SAMPLE_BUFFERS = GL_SAMPLE_BUFFERS,
    SAMPLE_COVERAGE_VALUE = GL_SAMPLE_COVERAGE_VALUE,
    SAMPLE_COVERAGE_INVERT = GL_SAMPLE_COVERAGE_INVERT,
    SAMPLER_BINDING = GL_SAMPLER_BINDING,
    SAMPLES = GL_SAMPLES,
    SCISSOR_BOX = GL_SCISSOR_BOX,
    SCISSOR_TEST = GL_SCISSOR_TEST,
    SHADER_COMPILER = GL_SHADER_COMPILER,
    SHADER_STORAGE_BUFFER_BINDING = GL_SHADER_STORAGE_BUFFER_BINDING,
    SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT = GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT,
    SHADER_STORAGE_BUFFER_START = GL_SHADER_STORAGE_BUFFER_START,
    SHADER_STORAGE_BUFFER_SIZE = GL_SHADER_STORAGE_BUFFER_SIZE,
    SMOOTH_LINE_WIDTH_RANGE = GL_SMOOTH_LINE_WIDTH_RANGE,
    SMOOTH_LINE_WIDTH_GRANULARITY = GL_SMOOTH_LINE_WIDTH_GRANULARITY,
    STENCIL_BACK_FAIL = GL_STENCIL_BACK_FAIL,
    STENCIL_BACK_FUNC = GL_STENCIL_BACK_FUNC,
    STENCIL_BACK_PASS_DEPTH_FAIL = GL_STENCIL_BACK_PASS_DEPTH_FAIL,
    STENCIL_BACK_PASS_DEPTH_PASS = GL_STENCIL_BACK_PASS_DEPTH_PASS,
    STENCIL_BACK_REF = GL_STENCIL_BACK_REF,
    STENCIL_BACK_VALUE_MASK = GL_STENCIL_BACK_VALUE_MASK,
    STENCIL_BACK_WRITEMASK = GL_STENCIL_BACK_WRITEMASK,
    STENCIL_CLEAR_VALUE = GL_STENCIL_CLEAR_VALUE,
    STENCIL_FAIL = GL_STENCIL_FAIL,
    STENCIL_FUNC = GL_STENCIL_FUNC,
    STENCIL_PASS_DEPTH_FAIL = GL_STENCIL_PASS_DEPTH_FAIL,
    STENCIL_PASS_DEPTH_PASS = GL_STENCIL_PASS_DEPTH_PASS,
    STENCIL_REF = GL_STENCIL_REF,
    STENCIL_TEST = GL_STENCIL_TEST,
    STENCIL_VALUE_MASK = GL_STENCIL_VALUE_MASK,
    STENCIL_WRITEMASK = GL_STENCIL_WRITEMASK,
    STEREO = GL_STEREO,
    SUBPIXEL_BITS = GL_SUBPIXEL_BITS,
    TEXTURE_BINDING_1D = GL_TEXTURE_BINDING_1D,
    TEXTURE_BINDING_1D_ARRAY = GL_TEXTURE_BINDING_1D_ARRAY,
    TEXTURE_BINDING_2D = GL_TEXTURE_BINDING_2D,
    TEXTURE_BINDING_2D_ARRAY = GL_TEXTURE_BINDING_2D_ARRAY,
    TEXTURE_BINDING_2D_MULTISAMPLE = GL_TEXTURE_BINDING_2D_MULTISAMPLE,
    TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY,
    TEXTURE_BINDING_3D = GL_TEXTURE_BINDING_3D,
    TEXTURE_BINDING_BUFFER = GL_TEXTURE_BINDING_BUFFER,
    TEXTURE_BINDING_CUBE_MAP = GL_TEXTURE_BINDING_CUBE_MAP,
    TEXTURE_BINDING_RECTANGLE = GL_TEXTURE_BINDING_RECTANGLE,
    TEXTURE_COMPRESSION_HINT = GL_TEXTURE_COMPRESSION_HINT,
    TEXTURE_BUFFER_OFFSET_ALIGNMENT = GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT,
    TIMESTAMP = GL_TIMESTAMP,
    TRANSFORM_FEEDBACK_BUFFER_BINDING = GL_TRANSFORM_FEEDBACK_BUFFER_BINDING,
    TRANSFORM_FEEDBACK_BUFFER_START = GL_TRANSFORM_FEEDBACK_BUFFER_START,
    TRANSFORM_FEEDBACK_BUFFER_SIZE = GL_TRANSFORM_FEEDBACK_BUFFER_SIZE,
    UNIFORM_BUFFER_BINDING = GL_UNIFORM_BUFFER_BINDING,
    UNIFORM_BUFFER_OFFSET_ALIGNMENT = GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,
    UNIFORM_BUFFER_SIZE = GL_UNIFORM_BUFFER_SIZE,
    UNIFORM_BUFFER_START = GL_UNIFORM_BUFFER_START,
    UNPACK_ALIGNMENT = GL_UNPACK_ALIGNMENT,
    UNPACK_IMAGE_HEIGHT = GL_UNPACK_IMAGE_HEIGHT,
    UNPACK_LSB_FIRST = GL_UNPACK_LSB_FIRST,
    UNPACK_ROW_LENGTH = GL_UNPACK_ROW_LENGTH,
    UNPACK_SKIP_IMAGES = GL_UNPACK_SKIP_IMAGES,
    UNPACK_SKIP_PIXELS = GL_UNPACK_SKIP_PIXELS,
    UNPACK_SKIP_ROWS = GL_UNPACK_SKIP_ROWS,
    UNPACK_SWAP_BYTES = GL_UNPACK_SWAP_BYTES,
    VERTEX_ARRAY_BINDING = GL_VERTEX_ARRAY_BINDING,
    VERTEX_BINDING_DIVISOR = GL_VERTEX_BINDING_DIVISOR,
    VERTEX_BINDING_OFFSET = GL_VERTEX_BINDING_OFFSET,
    VERTEX_BINDING_STRIDE = GL_VERTEX_BINDING_STRIDE,
    VERTEX_BINDING_BUFFER = GL_VERTEX_BINDING_BUFFER,
    MAX_VERTEX_ATTRIB_RELATIVE_OFFSET = GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET,
    MAX_VERTEX_ATTRIB_BINDINGS = GL_MAX_VERTEX_ATTRIB_BINDINGS,
    VIEWPORT = GL_VIEWPORT,
    VIEWPORT_BOUNDS_RANGE = GL_VIEWPORT_BOUNDS_RANGE,
    VIEWPORT_INDEX_PROVOKING_VERTEX = GL_VIEWPORT_INDEX_PROVOKING_VERTEX,
    VIEWPORT_SUBPIXEL_BITS = GL_VIEWPORT_SUBPIXEL_BITS,
    MAX_ELEMENT_INDEX = GL_MAX_ELEMENT_INDEX,
  };

  enum class ETextureCompareMode
  {
    NONE = GL_NONE,
    COMPARE_REF_TO_TEXTURE = GL_COMPARE_REF_TO_TEXTURE,
  };

  struct BlendFactors
  {
    GL::EBlendFactor mSourceBlendFactorRGB = GL::EBlendFactor::ONE;
    GL::EBlendFactor mDestBlendFactorRGB = GL::EBlendFactor::ZERO;
    GL::EBlendFactor mSourceBlendFactorAlpha = GL::EBlendFactor::ONE;
    GL::EBlendFactor mDestBlendFactorAlpha = GL::EBlendFactor::ZERO;
  };

  static void Enable(const GL::EEnablable inEnablable);
  static void Disable(const GL::EEnablable inEnablable);
  static void SetEnabled(const GL::EEnablable inEnablable, const bool inEnabled);
  static bool IsEnabled(const GL::EEnablable inEnablable);

  static void DepthMask(const bool inDepthMask);
  static bool GetDepthMask();

  static void DepthFunc(const GL::EDepthFunc inDepthFunc);
  static GL::EDepthFunc GetDepthFunc();

  static void BlendFunc(const GL::EBlendFactor inSourceBlendFactor, const GL::EBlendFactor inDestBlendFactor);
  static void BlendFuncSeparate(const GL::EBlendFactor inSourceBlendFactorRGB,
      const GL::EBlendFactor inDestBlendFactorRGB,
      const GL::EBlendFactor inSourceBlendFactorAlpha,
      const GL::EBlendFactor inDestBlendFactorAlpha);
  static void BlendFuncSeparate(const BlendFactors &inBlendFactors);
  static void BlendColor(const Color4f &inBlendColor);
  static GL::EBlendFactor GetSourceBlendFactorRGB();
  static GL::EBlendFactor GetDestBlendFactorRGB();
  static GL::EBlendFactor GetSourceBlendFactorAlpha();
  static GL::EBlendFactor GetDestBlendFactorAlpha();
  static Color4f GetBlendColor();

  static void PointSize(const float inPointSize);
  static float GetPointSize();

  static void LineWidth(const float inLineWidth);
  static float GetLineWidth();

  static void Viewport(const int inX, const int inY, const int inWidth, const int inHeight);
  static void Viewport(const Vec2i& inXY, const Vec2i& inSize);
  static void Viewport(const AARecti& inViewport);
  static AARecti GetViewport();

  static GL::Id GenBuffer();
  static GL::Id CreateBuffer();
  static void BindBuffer(const GL::EBufferType inBufferType, const GL::Id inBufferId);
  static void BindBufferBase(const GL::EBufferType inBufferType, const GL::Id inBindingPoint, const GL::Id inBufferId);
  template <typename T>
  static void BufferData(const GL::Id inBufferId, const Span<T>& inData, const GL::EAccessHint inAccessHint);
  template <typename T>
  static void BufferSubData(const GL::Id inBufferId, const Span<T>& inData, const GL::Size inOffset);
  static GL::EBindingType GetBufferBindingType(const GL::EBufferType inBufferType);
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
  static GL::Id CreateTexture(const GL::ETextureTarget& inTextureTarget);
  static void BindTexture(const GL::ETextureTarget& inTextureTarget, const GL::Id& inTextureId);
  template <typename T>
  static void TexImage2D(const GL::ETextureTarget& inTextureTarget,
      const GL::Size& inWidth,
      const GL::Size& inHeight,
      const GL::ETextureInputFormat& inInputFormat,
      const GL::ETextureInputComponentFormat& inInputComponentFormat,
      const Span<T>& inData,
      const GL::ETextureFormat& inTextureFormat,
      const GL::Int& inMipMapLevel = 0);
  template <typename T>
  static std::vector<T> GetTextureImage(const GL::Id inTextureId,
      const GL::ETextureInputFormat inFormatToConvertTo,
      const GL::EDataType inDataTypeToConvertTo,
      const GL::Size inNumberOfTexelsToRead,
      const GL::Int inMipmapLevel = 0);
  static void TextureParameteri(const GL::Id inTextureId,
      const GL::ETextureParameter inTextureParameter,
      const GL::Int inParameter);
  static void SetTextureCompareMode(const GL::Id inTextureId, const GL::ETextureCompareMode inTextureCompareMode);
  static GL::Int GetTextureParameteri(const GL::Id inTextureId, const GL::ETextureParameter inTextureParameter);
  static void GenerateTextureMipMap(const GL::Id& inTextureId);
  static void ActiveTexture(const GL::Id& inTextureUnit);
  static void BindTextureUnit(const GL::Size& inTextureUnit, const GL::Id& inTextureId);
  static bool IsColorFormat(const GL::ETextureFormat inTextureFormat);
  static bool IsDepthOnlyFormat(const GL::ETextureFormat inTextureFormat);
  static bool IsDepthStencilFormat(const GL::ETextureFormat inTextureFormat);
  static void DeleteTexture(const GL::Id& inTextureId);

  static GL::Id GenFramebuffer();
  static GL::Id CreateFramebuffer();
  static void BindFramebuffer(const GL::Id inFramebufferId);
  static void FramebufferRenderbuffer(const GL::EFramebufferAttachment inAttachment, const GL::Id inRenderbufferId);
  static void FramebufferTexture2D(const GL::EFramebufferAttachment inAttachment,
      const GL::ETextureTarget inTextureTarget,
      const GL::Id inTextureId,
      const GL::Size inMipmapLevel = 0);
  static void BlitFramebuffer(const GL::Id inReadFramebufferId,
      const Vec2i& inReadMin,
      const Vec2i& inReadMax,
      const GL::Id inDrawFramebufferId,
      const Vec2i& inDrawMin,
      const Vec2i& inDrawMax,
      const GL::EBufferBitFlags inBufferMask,
      const GL::EFilterType inFilterType);
  static void DeleteFramebuffer(const GL::Id inFramebufferId);

  static GL::Id GenRenderbuffer();
  static GL::Id CreateRenderbuffer();
  static void RenderbufferStorage(const GL::ETextureFormat inFormat, const GL::Size inWidth, const GL::Size inHeight);
  static void BindRenderbuffer(const GL::Id inRenderbufferId);
  static void DeleteRenderbuffer(const GL::Id inRenderbufferId);

  static void ClearColor(const Color4f& inClearColor);
  static void ClearDepth(const float inClearDepth = 1.0f);
  static void Clear(const GL::EBufferBitFlags& inBufferBitFlags);
  static void DrawElements(const GL::EPrimitivesType inPrimitivesType,
      const GL::Size inNumberOfPrimitives,
      const GL::EDataType inIndicesDataType,
      const GL::Size inBeginPrimiviteIndex = 0);
  static void DrawArrays(const GL::EPrimitivesType inPrimitivesType,
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
  static GL::Id GetUniformBlockIndex(const GL::Id inShaderProgramId, const std::string_view inUniformBlockName);
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
  static void
  UniformBlockBinding(const GL::Id inShaderProgramId, const GL::Id inUniformBlockIndex, const GL::Id inBindingPoint);
  static void DeleteProgram(const GL::Id inShaderProgramId);

  static bool GetBoolean(const GL::EGetEnum inGetBooleanId);
  static GL::Int GetInteger(const GL::EGetEnum inGetIntegerId);
  static GL::Float GetFloat(const GL::EGetEnum inGetFloatId);
  static GL::Double GetDouble(const GL::EGetEnum inGetDoubleId);

  template <std::size_t N>
  static std::array<GL::Int, N> GetIntegers(const GL::EGetEnum inGetIntegerId);

  template <std::size_t N>
  static std::array<GL::Float, N> GetFloats(const GL::EGetEnum inGetFloatId);

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

  // Generic Create/Delete
  template <GL::EObjectType TObjectType>
  static GL::Id Create();
  template <GL::EObjectType TObjectType>
  static void Delete(const GL::Id inId);

  // Generic Bind/UnBind
  template <GL::EBindingType TBindingType>
  static void Bind(const GL::Id inId);
  template <GL::EBindingType TBindingType>
  static void UnBind();
  static GL::Id GetBoundGLId(const GL::EBindingType inBindingType)
  {
    return GL::GetInteger(static_cast<GL::EGetEnum>(inBindingType));
  }

  // To ObjectType conversions
  template <GL::EBindingType TBindingType>
  static constexpr GL::EObjectType GetObjectType();
  template <GL::EShaderType TShaderType>
  static constexpr GL::EObjectType GetObjectType();
  template <GL::ETextureTarget TTextureTarget>
  static constexpr GL::EObjectType GetObjectType();
  template <GL::EBufferType TBufferType>
  static constexpr GL::EObjectType GetObjectType();

  // To BindingType conversions
  template <GL::EObjectType TObjectType>
  static constexpr GL::EBindingType GetBindingType();
  template <GL::EShaderType TShaderType>
  static constexpr GL::EBindingType GetBindingType();
  template <GL::ETextureTarget TTextureTarget>
  static constexpr GL::EBindingType GetBindingType();
  template <GL::EBufferType TBufferType>
  static constexpr GL::EBindingType GetBindingType();

  GL() = delete; // Static class
};

// clang-format off
#define GL_SAFE_CALL(EXPR) [&](){ GL::ClearError(); EXPR; const auto error = GL::CheckError(); if (error != GL::EError::NO_ERROR) THROW_EXCEPTION("GL error: " << GL::ErrorToString(error)); }();
#define GL_SAFE_CALL_RET(EXPR) [&](){ GL::ClearError(); const auto ret = EXPR; const auto error = GL::CheckError(); if (error != GL::EError::NO_ERROR) THROW_EXCEPTION("GL error: " << GL::ErrorToString(error)); return ret; }();
// clang-format on

DECLARE_FLAGS(GL::EBufferBitFlags);

}

#include "ez/GL.tcc"