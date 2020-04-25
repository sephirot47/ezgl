#pragma once

#include "GL.h"
#include <type_traits>

namespace egl
{

class GLNoOpGuard final
{
};

template <GL::EBindingType TBindingType>
class GLBindGuard final
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

// clang-format off
template <GL::EBindingType TBindingType> decltype(auto) GetGLBindGuard() { return GLBindGuard<TBindingType>(); }
template <typename T> decltype(auto) GetGLBindGuard() { return typename T::GLBindGuardType {}; }

#define GL_BIND_GUARD(TYPE) const auto ANONYMOUS_VARIABLE_NAME() = GetGLBindGuard<TYPE>();
#define GL_BIND_GUARD_VAR(VARIABLE) GL_BIND_GUARD(std::remove_reference_t<std::remove_const_t<decltype(VARIABLE)>>)
// clang-format on

template <typename... TGuards>
class GLCompositeGuard : public std::tuple<decltype(GetGLBindGuard<TGuards>())...>
{
public:
  GLCompositeGuard() = default;
  GLCompositeGuard(const GLCompositeGuard& inRHS) = delete;
  GLCompositeGuard& operator=(const GLCompositeGuard& inRHS) = delete;
  GLCompositeGuard(GLCompositeGuard&& ioRHS) = default;
  GLCompositeGuard& operator=(GLCompositeGuard&& ioRHS) = delete;
};
// GLCompositeGuard use example: GLCompositeGuard<VAO, ShaderProgram, VBO>

}