#pragma once

#include "GL.h"
#include <functional>
#include <type_traits>

namespace egl
{

class GLNoOpGuard final
{
};

template <typename TValue, auto TGetFunction, auto TSetFunction>
class GLGenericGuard final
{
public:
  GLGenericGuard() : mValueOpt(TGetFunction()) {}
  GLGenericGuard(const GLGenericGuard& inRHS) = delete;
  GLGenericGuard& operator=(const GLGenericGuard& inRHS) = delete;
  GLGenericGuard(GLGenericGuard&& ioRHS)
  {
    EXPECTS(!mValueOpt.has_value());
    std::swap(mValueOpt, ioRHS.mValueOpt);
  }
  GLGenericGuard& operator=(GLGenericGuard&& ioRHS) = delete;

  ~GLGenericGuard()
  {
    if (mValueOpt.has_value())
      TSetFunction(*mValueOpt);
  }

private:
  std::optional<TValue> mValueOpt = std::nullopt;
};

// clang-format off

// GLBindGuard
template <GL::EBindingType TBindingType> GL::Id GLBindGuardGetFunction() { return GL::GetBoundGLId(TBindingType); }
template <GL::EBindingType TBindingType> void GLBindGuardSetFunction(const GL::Id inId) { GL::Bind<TBindingType>(inId); }
template <GL::EBindingType TBindingType> using GLBindGuard = GLGenericGuard<GL::Id, GLBindGuardGetFunction<TBindingType>, GLBindGuardSetFunction<TBindingType>>;

// GLEnableGuard
template <GL::EEnablable TEnablable> GL::Id GLEnableGuardGetFunction() { return GL::IsEnabled(TEnablable); }
template <GL::EEnablable TEnablable> void GLEnableGuardSetFunction(const bool inEnable) { GL::SetEnabled(TEnablable, inEnable); }
template <GL::EEnablable TEnablable> using GLEnableGuard = GLGenericGuard<bool, GLEnableGuardGetFunction<TEnablable>, GLEnableGuardSetFunction<TEnablable>>;

// GetGLGuard
template <typename T> decltype(auto) GetGLGuard() { return typename T::GLGuardType{}; };
template <GL::EBindingType TBindingTypeValue> decltype(auto) GetGLGuard() { return GLBindGuard<TBindingTypeValue>{}; };
template <GL::EEnablable TEnablableValue> decltype(auto) GetGLGuard() { return GLEnableGuard<TEnablableValue>{}; };

// GLGuardWrap
template <auto TGLGuardEnumClassValue> struct GLGuardWrap { using GLGuardWrap_Type = decltype(GetGLGuard<TGLGuardEnumClassValue>()); };
template <auto TGLGuardEnumClassValue> using GLGuardWrap_t = typename GLGuardWrap<TGLGuardEnumClassValue>::GLGuardWrap_Type;

// Helper macros to create guards
#define GL_GUARD(TYPE) const auto ANONYMOUS_VARIABLE_NAME = GetGLGuard<TYPE>();
#define GL_GUARD_VAR(VARIABLE) GL_GUARD(std::remove_reference_t<std::remove_const_t<decltype(VARIABLE)>>)


// GLMultiGuard
// Example. If you want to use values, they must be wrapped with GLGuardWrap_t:
//     GLMultiGuard<VAO::GLGuardType, GLGuardWrap_t<GL::Enablable::DEPTH_TEST>, ShaderProgram::GLGuardType, GLGuardWrap_t<GL::EBindingType::UBO>> guard;
template <typename... TGLGuards>
using GLMultiGuard = std::tuple<TGLGuards...>;

// clang-format on
}