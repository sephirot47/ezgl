#pragma once

#include "GL.h"

namespace egl
{

template <typename... TGuards>
class GLComplexGuard
{
private:
  std::tuple<TGuards...> mGuards;
};

template <GL::EBindingType TBindingType>
class GLBindGuard
{
public:
  GLBindGuard() : mPreviouslyBoundId(GL::GetBoundGLId(TBindingType)) {}
  ~GLBindGuard() { GL::Bind<TBindingType>(mPreviouslyBoundId); }

private:
  const GL::Id mPreviouslyBoundId = 0;
};

template <GL::EBindingType TBindingType>
decltype(auto) GetGLBindGuard()
{
  return GLBindGuard<TBindingType>();
}

template <typename T>
decltype(auto) GetGLBindGuard()
{
  return typename T::GLBindGuardType {};
}

#define GL_BIND_GUARD(TYPE) const auto ANONYMOUS_VARIABLE_NAME() = GetGLBindGuard<TYPE>();
}