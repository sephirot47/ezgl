#pragma once

#include "GL.h"
#include "GLObject.h"

namespace egl
{
template <GL::EBindingType TBindingType>
class GLBindableObject : public GLObject<GL::GetObjectType<TBindingType>()>
{
public:
  static constexpr auto ObjectType = GL::GetObjectType<TBindingType>();
  static constexpr auto BindingType = TBindingType;
  using GLObjectBase = GLObject<ObjectType>;
  using GLObjectBase::GetGLId;

  GLBindableObject() = default;
  GLBindableObject(GLBindableObject&& ioRHS) noexcept = default;
  virtual ~GLBindableObject() = default;

  void Bind() const;
  void UnBind() const;
  bool IsBound() const;
  static GL::Id GetBoundGLId();
};
}

#include "GLBindableObject.tcc"