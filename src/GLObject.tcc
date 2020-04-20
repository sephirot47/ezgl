#include "GLObject.h"

namespace egl
{

template <GL::EObjectType TObjectType>
GLObject<TObjectType>::GLObject() : mGLId(GL::Create<TObjectType>())
{
}

template <GL::EObjectType TObjectType>
GLObject<TObjectType>::GLObject(GLObject&& ioRHS) noexcept
{
  EXPECTS(mGLId == 0);
  std::swap(mGLId, ioRHS.mGLId);
}

template <GL::EObjectType TObjectType>
GLObject<TObjectType>::~GLObject()
{
  if (mGLId != 0)
    GL::Delete<TObjectType>(mGLId);
}
template <GL::EObjectType TObjectType>
GL::Id GLObject<TObjectType>::GetGLId() const
{
  return mGLId;
}

}