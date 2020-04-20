#include "GLBindableObject.h"

namespace egl
{
template <GL::EBindingType TBindingType>
void GLBindableObject<TBindingType>::Bind() const
{
  GL::Bind<TBindingType>(GetGLId());
}

template <GL::EBindingType TBindingType>
void GLBindableObject<TBindingType>::UnBind() const
{
  GL::UnBind<TBindingType>();
}

template <GL::EBindingType TBindingType>
bool GLBindableObject<TBindingType>::IsBound() const
{
  return ((GetGLId() != 0) && (GetGLId() == GLBindableObject<TBindingType>::GetBoundGLId()));
}

template <GL::EBindingType TBindingType>
GL::Id GLBindableObject<TBindingType>::GetBoundGLId()
{
  return GL::GetBoundGLId(TBindingType);
}
}