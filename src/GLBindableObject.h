#pragma once

#include "GL.h"
#include "GLGuard.h"
#include "GLObject.h"

namespace egl
{
template <GL::EBindingType TBindingType, typename TGLGuard = GLBindGuard<TBindingType>>
class GLBindableObject : public GLObject<GL::GetObjectType<TBindingType>()>
{
public:
  static constexpr auto ObjectType = GL::GetObjectType<TBindingType>();
  static constexpr auto BindingType = TBindingType;
  using GLObjectBase = GLObject<ObjectType>;
  using GLObjectBase::GetGLId;
  using GLGuardType = TGLGuard;

  GLBindableObject() = default;
  GLBindableObject(GLBindableObject&& ioRHS) noexcept = default;
  virtual ~GLBindableObject() = default;

  void Bind() const;
  [[nodiscard]] GLGuardType BindGuarded() const;
  void UnBind() const;
  bool IsBound() const;
  static GL::Id GetBoundGLId();
  [[nodiscard]] static GLGuardType CreateBindGuard() { return GLGuardType {}; }
};
}

#include "GLBindableObject.tcc"