#pragma once

#include "GL.h"

namespace egl
{
template <GL::EBindingType TBindingType>
class GLObject
{
public:
  static constexpr auto BindingType = TBindingType;

  GLObject();
  GLObject(const GLObject& inRHS) = delete;
  GLObject& operator=(const GLObject& inRHS) = delete;
  GLObject(GLObject&& ioRHS) noexcept;
  GLObject& operator=(GLObject&& ioRHS) = delete;
  virtual ~GLObject();

  void Bind() const;
  void UnBind() const;
  bool IsBound() const;
  static GL::Id GetBoundGLId();

  GL::Id GetGLId() const;

private:
  GL::Id mGLId = 0;
};
}

#include "GLObject.tcc"