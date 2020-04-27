#pragma once

#include "GL.h"
#include <functional>
#include <type_traits>

namespace egl
{

class GLNoOpGuard final
{
};

template <typename TValue, auto TGetFunction, auto TSetFunction, typename... TExtraValues>
class GLGenericGuard final
{
public:
  GLGenericGuard(TExtraValues&&... ioExtraValues) : mExtraValues(std::forward<TExtraValues>(ioExtraValues)...)
  {
    if constexpr (sizeof...(TExtraValues) == 0)
    {
      mValueOpt = TGetFunction();
    }
    else
    {
      mValueOpt = TGetFunction(mExtraValues);
    }
  }
  GLGenericGuard(const GLGenericGuard& inRHS) = delete;
  GLGenericGuard& operator=(const GLGenericGuard& inRHS) = delete;
  GLGenericGuard(GLGenericGuard&& ioRHS)
  {
    EXPECTS(!mValueOpt.has_value());
    mExtraValues = std::move(ioRHS.mExtraValues);
    std::swap(mValueOpt, ioRHS.mValueOpt);
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
template <GL::EBindingType TBindingType> using GLBindGuard = GLGenericGuard<GL::Id, GLBindGuardGet<TBindingType>, GLBindGuardSet<TBindingType>>;

// GLEnableGuard
template <GL::EEnablable TEnablable> GL::Id GLEnableGuardGet() { return GL::IsEnabled(TEnablable); }
template <GL::EEnablable TEnablable> void GLEnableGuardSet(const bool inEnable) { GL::SetEnabled(TEnablable, inEnable); }
template <GL::EEnablable TEnablable> using GLEnableGuard = GLGenericGuard<bool, GLEnableGuardGet<TEnablable>, GLEnableGuardSet<TEnablable>>;

// GLDepthMaskGuard
inline bool GLDepthMaskGuardGet() { return GL::GL::GetDepthMask(); }
inline void GLDepthMaskGuardSet(const bool inDepthMask) { GL::DepthMask(inDepthMask); }
using GLDepthMaskGuard = GLGenericGuard<bool, GLDepthMaskGuardGet, GLDepthMaskGuardSet>;

// GLDepthFuncGuard
inline GL::EDepthFunc GLDepthFuncGuardGet() { return GL::GetDepthFunc(); }
inline void GLDepthFuncGuardSet(const GL::EDepthFunc inDepthFunc) { GL::DepthFunc(inDepthFunc); }
using GLDepthFuncGuard = GLGenericGuard<GL::EDepthFunc, GLDepthFuncGuardGet, GLDepthFuncGuardSet>;

// GLBlendFuncGuard
inline std::array<GL::EBlendFactor, 4> GLBlendFuncGuardGet()
{ return std::array { GL::GetSourceBlendFactorRGB(), GL::GetDestBlendFactorRGB(), GL::GetSourceBlendFactorAlpha(), GL::GetDestBlendFactorAlpha() }; }
inline void GLBlendFuncGuardSet(const std::array<GL::EBlendFactor, 4>& inBlendFactors) { GL::BlendFuncSeparate(inBlendFactors[0], inBlendFactors[1], inBlendFactors[2], inBlendFactors[3]); }
using GLBlendFuncGuard = GLGenericGuard<std::array<GL::EBlendFactor, 4>, GLBlendFuncGuardGet, GLBlendFuncGuardSet>;

// GLTextureParameteriGuard
template <GL::ETextureParameter TTextureParameter> inline GL::Int GLTextureParameteriGuardGet(const std::tuple<GL::Id>& inTextureId) { return GL::GetTextureParameteri(std::get<0>(inTextureId), TTextureParameter); }
template <GL::ETextureParameter TTextureParameter> inline void GLTextureParameteriGuardSet(const GL::Int inValue, const std::tuple<GL::Id>& inTextureId) { GL::TextureParameteri(std::get<0>(inTextureId), TTextureParameter, inValue); }
template <GL::ETextureParameter TTextureParameter> using GLTextureParameteriGuard = GLGenericGuard<GL::Int, GLTextureParameteriGuardGet<TTextureParameter>, GLTextureParameteriGuardSet<TTextureParameter>, GL::Id>;

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