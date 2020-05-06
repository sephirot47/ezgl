#include "ezgl/GLBindableObject.h"

namespace ez
{
template <GL::EBindingType TBindingType, typename TGLGuard>
void GLBindableObject<TBindingType, TGLGuard>::Bind() const
{
  GL::Bind<TBindingType>(GetGLId());
}

template <GL::EBindingType TBindingType, typename TGLGuard>
TGLGuard GLBindableObject<TBindingType, TGLGuard>::BindGuarded() const
{
  TGLGuard guard;
  GL::Bind<TBindingType>(GetGLId());
  return guard;
}

template <GL::EBindingType TBindingType, typename TGLGuard>
void GLBindableObject<TBindingType, TGLGuard>::UnBind() const
{
  GL::UnBind<TBindingType>();
}

template <GL::EBindingType TBindingType, typename TGLGuard>
bool GLBindableObject<TBindingType, TGLGuard>::IsBound() const
{
  return ((GetGLId() != 0) && (GetGLId() == GLBindableObject<TBindingType, TGLGuard>::GetBoundGLId()));
}

template <GL::EBindingType TBindingType, typename TGLGuard>
GL::Id GLBindableObject<TBindingType, TGLGuard>::GetBoundGLId()
{
  return GL::GetBoundGLId(TBindingType);
}
}