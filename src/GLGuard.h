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
auto GetGLGuard()
{
  return GLComplexGuard<GLBindGuard<TBindingType>>();
}

template <typename T>
struct GLGuardReturnType
{
  using type = T;
};
template <typename T>
using GLGuardReturnType_t = typename GLGuardReturnType<T>::type;

template <typename T>
GLGuardReturnType_t<T> GetGLGuard()
{
  static_assert(std::is_same_v<T, T>, "GLGuard not specialized for this type.");
}

#define GL_BIND_GUARD(TYPE) const auto ANONYMOUS_VARIABLE_NAME() = GetGLGuard<TYPE>();
}