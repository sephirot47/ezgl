#pragma once

#include "ez/GL.h"
#include <functional>
#include <type_traits>

namespace ez
{

class GLNoOpGuard final
{
};

template <auto TGetFunction, auto TSetFunction, typename... TExtraValues>
class GLGenericGuard final
{
public:
  using TValue = std::remove_const_t<std::remove_reference_t<decltype(TGetFunction(std::declval<TExtraValues>()...))>>;

  GLGenericGuard(TExtraValues&&... ioExtraValues)
      : mExtraValues(std::forward<TExtraValues>(ioExtraValues)...), mValueOpt([&]() {
          if constexpr (sizeof...(TExtraValues) == 0)
          {
            return TGetFunction();
          }
          else
          {
            return TGetFunction(mExtraValues);
          }
        }())
  {
    ENSURES(mValueOpt.has_value());
  }

  GLGenericGuard(const GLGenericGuard& inRHS) = delete;
  GLGenericGuard& operator=(const GLGenericGuard& inRHS) = delete;

  GLGenericGuard(GLGenericGuard&& ioRHS)
  {
    EXPECTS(!mValueOpt.has_value());
    mExtraValues = std::move(ioRHS.mExtraValues);
    mValueOpt = std::move(ioRHS.mValueOpt);
    ENSURES(mValueOpt.has_value());
  }
  GLGenericGuard& operator=(GLGenericGuard&& ioRHS) = delete;

  ~GLGenericGuard()
  {
    if (mValueOpt.has_value())
    {
      if constexpr (sizeof...(TExtraValues) == 0)
      {
        TSetFunction(*mValueOpt);
      }
      else
      {
        TSetFunction(*mValueOpt, mExtraValues);
      }
    }
  }

  std::tuple<TExtraValues...> mExtraValues;       // Order dependent because of initialization
  std::optional<TValue> mValueOpt = std::nullopt; // Order dependent because of initialization
};

// clang-format off

// GLBindGuard
template <GL::EBindingType TBindingType> GL::Id GLBindGuardGet() { return GL::GetBoundGLId(TBindingType); }
template <GL::EBindingType TBindingType> void GLBindGuardSet(const GL::Id inId) { GL::Bind<TBindingType>(inId); }
template <GL::EBindingType TBindingType> using GLBindGuard = GLGenericGuard<GLBindGuardGet<TBindingType>, GLBindGuardSet<TBindingType>>;

// GLEnableGuard
template <GL::EEnablable TEnablable> GL::Id GLEnableGuardGet() { return GL::IsEnabled(TEnablable); }
template <GL::EEnablable TEnablable> void GLEnableGuardSet(const bool inEnable) { GL::SetEnabled(TEnablable, inEnable); }
template <GL::EEnablable TEnablable> using GLEnableGuard = GLGenericGuard<GLEnableGuardGet<TEnablable>, GLEnableGuardSet<TEnablable>>;

// GLDepthMaskGuard
inline bool GLDepthMaskGuardGet() { return GL::GetDepthMask(); }
inline void GLDepthMaskGuardSet(const bool inDepthMask) { GL::DepthMask(inDepthMask); }
using GLDepthMaskGuard = GLGenericGuard<GLDepthMaskGuardGet, GLDepthMaskGuardSet>;

// GLDepthFuncGuard
inline GL::EDepthFunc GLDepthFuncGuardGet() { return GL::GetDepthFunc(); }
inline void GLDepthFuncGuardSet(const GL::EDepthFunc inDepthFunc) { GL::DepthFunc(inDepthFunc); }
using GLDepthFuncGuard = GLGenericGuard<GLDepthFuncGuardGet, GLDepthFuncGuardSet>;

// GLBlendFuncGuard
inline std::array<GL::EBlendFactor, 4> GLBlendFuncGuardGet()
{ return std::array { GL::GetSourceBlendFactorRGB(), GL::GetDestBlendFactorRGB(), GL::GetSourceBlendFactorAlpha(), GL::GetDestBlendFactorAlpha() }; }
inline void GLBlendFuncGuardSet(const std::array<GL::EBlendFactor, 4>& inBlendFactors) { GL::BlendFuncSeparate(inBlendFactors[0], inBlendFactors[1], inBlendFactors[2], inBlendFactors[3]); }
using GLBlendFuncGuard = GLGenericGuard<GLBlendFuncGuardGet, GLBlendFuncGuardSet>;

// GLTextureParameteriGuard
template <GL::ETextureParameter TTextureParameter> inline GL::Int GLTextureParameteriGuardGet(const std::tuple<GL::Id>& inTextureId) { return GL::GetTextureParameteri(std::get<0>(inTextureId), TTextureParameter); }
template <GL::ETextureParameter TTextureParameter> inline void GLTextureParameteriGuardSet(const GL::Int inValue, const std::tuple<GL::Id>& inTextureId) { GL::TextureParameteri(std::get<0>(inTextureId), TTextureParameter, inValue); }
template <GL::ETextureParameter TTextureParameter> using GLTextureParameteriGuard = GLGenericGuard<GLTextureParameteriGuardGet<TTextureParameter>, GLTextureParameteriGuardSet<TTextureParameter>, GL::Id>;

// GLViewportGuard
inline Recti GLViewportGuardGet() { return GL::GetViewport(); }
inline void GLViewportGuardSet(const Recti &inPreviousViewport) { GL::Viewport(inPreviousViewport); }
using GLViewportGuard = GLGenericGuard<GLViewportGuardGet, GLViewportGuardSet>;

// GLPointSizeGuard
inline float GLPointSizeGuardGet() { return GL::GetPointSize(); }
inline void GLPointSizeGuardSet(const float inPreviousPointSize) { GL::PointSize(inPreviousPointSize); }
using GLPointSizeGuard = GLGenericGuard<GLPointSizeGuardGet, GLPointSizeGuardSet>;

// GLLineWidthGuard
inline float GLLineWidthGuardGet() { return GL::GetLineWidth(); }
inline void GLLineWidthGuardSet(const float inPreviousLineWidth) { GL::LineWidth(inPreviousLineWidth); }
using GLLineWidthGuard = GLGenericGuard<GLLineWidthGuardGet, GLLineWidthGuardSet>;


// GetGLGuard
template <typename T> decltype(auto) GetGLGuard() { return typename T::GLGuardType{}; };
template <GL::EBindingType TBindingTypeValue> decltype(auto) GetGLGuard() { return GLBindGuard<TBindingTypeValue>{}; };
template <GL::EEnablable TEnablableValue> decltype(auto) GetGLGuard() { return GLEnableGuard<TEnablableValue>{}; };

// GLGuardWrap
template <auto TGLGuardEnumClassValue> struct GLGuardWrap { using GLGuardWrap_Type = decltype(GetGLGuard<TGLGuardEnumClassValue>()); };
template <auto TGLGuardEnumClassValue> using GLGuardWrap_t = typename GLGuardWrap<TGLGuardEnumClassValue>::GLGuardWrap_Type;

// GLMultiGuard
// Example. If you want to use values, they must be wrapped with GLGuardWrap_t:
//     GLMultiGuard<VAO::GLGuardType, GLGuardWrap_t<GL::Enablable::DEPTH_TEST>, ShaderProgram::GLGuardType, GLGuardWrap_t<GL::EBindingType::UBO>> guard;
template <typename... TGLGuards>
using GLMultiGuard = std::tuple<TGLGuards...>;

// clang-format on
}