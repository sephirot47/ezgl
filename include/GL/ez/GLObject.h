#pragma once

#include "ez/GL.h"

namespace ez
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

#include "ez/GLObject.tcc"