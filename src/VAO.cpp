#include "VAO.h"
#include "EBO.h"
#include "GL.h"
#include "GLGuard.h"
#include "Macros.h"
#include "VBO.h"
#include <GL/glew.h>

namespace egl
{
void VAO::AddVBO(const std::shared_ptr<VBO>& inVBO,
    const GL::Id inAttribLocation,
    const VAOVertexAttrib& inVertexAttrib)
{
  GL_BIND_GUARD(VBO); // First VBO so that the guard is released the latest
  GL_BIND_GUARD(VAO);
  Bind();

  mVBOs.push_back(inVBO);
  inVBO->Bind();
  AddVertexAttrib(inAttribLocation, inVertexAttrib);
}

void VAO::SetEBO(const std::shared_ptr<EBO>& inEBO)
{
  GL_BIND_GUARD(EBO); // First EBO so that the guard is released the latest
  GL_BIND_GUARD(VAO);
  Bind();

  mEBO = inEBO;
  inEBO->Bind();
}

void VAO::AddVertexAttrib(const GL::Id inAttribLocation, const VAOVertexAttrib& inVertexAttrib)
{
  EXPECTS(inVertexAttrib.mNumComponents > 0);

  GL_BIND_GUARD(VAO);
  Bind();

  GL::EnableVertexAttribArray(inAttribLocation);
  GL::VertexAttribPointer(inAttribLocation,
      inVertexAttrib.mNumComponents,
      inVertexAttrib.mType,
      inVertexAttrib.mNormalized,
      inVertexAttrib.mStride,
      inVertexAttrib.mOffset);
}

void VAO::RemoveVertexAttrib(const GL::Id inAttribLocation)
{
  EXPECTS(inAttribLocation > 0);

  GL_BIND_GUARD(VAO);
  Bind();

  GL::DisableVertexAttribArray(inAttribLocation);
}

const std::shared_ptr<EBO>& VAO::GetEBO() const { return mEBO; }

const std::vector<std::shared_ptr<VBO>>& VAO::GetVBOs() const { return mVBOs; }
}
