#pragma once

#include "GL.h"
#include <type_traits>

namespace egl
{

class GLGuardBase
{
public:
  using GLGuardTrait = void;
};

class GLNoOpGuard final : public GLGuardBase
{
};

template <GL::EBindingType TBindingType>
class GLBindGuard final : public GLGuardBase
{
public:
  GLBindGuard() : mPreviouslyBoundId(GL::GetBoundGLId(TBindingType)) {}
  GLBindGuard(const GLBindGuard& inRHS) = delete;
  GLBindGuard& operator=(const GLBindGuard& inRHS) = delete;
  GLBindGuard(GLBindGuard&& ioRHS)
  {
    EXPECTS(mPreviouslyBoundId == GL::InvalidId);
    std::swap(mPreviouslyBoundId, ioRHS.mPreviouslyBoundId);
  }
  GLBindGuard& operator=(GLBindGuard&& ioRHS) = delete;

  ~GLBindGuard()
  {
    if (mPreviouslyBoundId != GL::InvalidId)
      GL::Bind<TBindingType>(mPreviouslyBoundId);
  }

private:
  GL::Id mPreviouslyBoundId = GL::InvalidId;
};

template <GL::EEnablable TEnablable>
class GLEnableGuard final : public GLGuardBase
{
public:
  GLEnableGuard() : mWasEnabledOpt(GL::IsEnabled(TEnablable)) {}
  GLEnableGuard(const GLEnableGuard& inRHS) = delete;
  GLEnableGuard& operator=(const GLEnableGuard& inRHS) = delete;
  GLEnableGuard(GLEnableGuard&& ioRHS)
  {
    EXPECTS(!mWasEnabledOpt.has_value());
    std::swap(mWasEnabledOpt, ioRHS.mWasEnabledOpt);
  }
  GLEnableGuard& operator=(GLEnableGuard&& ioRHS) = delete;

  ~GLEnableGuard()
  {
    if (mWasEnabledOpt.has_value())
      GL::SetEnabled(TEnablable, *mWasEnabledOpt);
  }

private:
  std::optional<bool> mWasEnabledOpt = std::nullopt;
};

// clang-format off

template <typename T> decltype(auto) GetGLGuard() { return typename T::GLGuardType{}; };
template <GL::EBindingType TBindingTypeValue> decltype(auto) GetGLGuard() { return GLBindGuard<TBindingTypeValue>{}; };
template <GL::EEnablable TEnablableValue> decltype(auto) GetGLGuard() { return GLEnableGuard<TEnablableValue>{}; };

template <auto TGLGuardEnumClassValue> struct GLGuardWrap { using GLGuardWrap_Type = decltype(GetGLGuard<TGLGuardEnumClassValue>()); };
template <auto TGLGuardEnumClassValue> using GLGuardWrap_t = typename GLGuardWrap<TGLGuardEnumClassValue>::GLGuardWrap_Type;

#define GL_GUARD(TYPE) const auto ANONYMOUS_VARIABLE_NAME = GetGLGuard<TYPE>();
#define GL_GUARD_VAR(VARIABLE) GL_GUARD(std::remove_reference_t<std::remove_const_t<decltype(VARIABLE)>>)

template <typename... TGLGuards>
using GLMultiGuard = std::tuple<TGLGuards...>;
// Example. If you want to use values, they must be wrapped with GLGuardWrap_t:
//     GLMultiGuard<VAO::GLGuardType, GLGuardWrap_t<GL::Enablable::DEPTH_TEST>, ShaderProgram::GLGuardType, GLGuardWrap_t<GL::EBindingType::UBO>> guard;

// clang-format on
}