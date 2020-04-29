#pragma once

#include "ezgl/GL.h"

namespace egl
{
template <GL::EObjectType TObjectType>
class GLObject
{
public:
  GLObject();
  GLObject(const GLObject& inRHS) = delete;
  GLObject& operator=(const GLObject& inRHS) = delete;
  GLObject(GLObject&& ioRHS) noexcept;
  GLObject& operator=(GLObject&& ioRHS) = delete;
  virtual ~GLObject();

  GL::Id GetGLId() const;

private:
  GL::Id mGLId = 0;
};
}

#include "ezgl/GLObject.tcc"