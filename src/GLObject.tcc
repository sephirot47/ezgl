#include "GLObject.h"

namespace egl
{

template <GL::EBindingType TBindingType>
GLObject<TBindingType>::GLObject() : mGLId(GL::Create<TBindingType>())
{
}

template <GL::EBindingType TBindingType>
GLObject<TBindingType>::GLObject(GLObject&& ioRHS) noexcept
{
  EXPECTS(mGLId == 0);
  std::swap(mGLId, ioRHS.mGLId);
}

template <GL::EBindingType TBindingType>
GLObject<TBindingType>::~GLObject()
{
  if (mGLId != 0)
    GL::Delete<TBindingType>(mGLId);
}
template <GL::EBindingType TBindingType>
void GLObject<TBindingType>::Bind() const
{
  GL::Bind<TBindingType>(mGLId);
}

template <GL::EBindingType TBindingType>
void GLObject<TBindingType>::UnBind() const
{
  GL::UnBind<TBindingType>(mGLId);
}

template <GL::EBindingType TBindingType>
bool GLObject<TBindingType>::IsBound() const
{
  return ((mGLId != 0) && (mGLId == GLObject<TBindingType>::GetBoundGLId()));
}

template <GL::EBindingType TBindingType>
GL::Id GLObject<TBindingType>::GetBoundGLId()
{
  return GL::GetBoundGLId(TBindingType);
}

template <GL::EBindingType TBindingType>
GL::Id GLObject<TBindingType>::GetGLId() const
{
  return mGLId;
}

}