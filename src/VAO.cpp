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
  const GLBindGuard<GL::EBindingType::VBO> vbo_bind_guard; // First VBO so that the guard is released the latest
  const auto vao_bind_guard = BindGuarded();

  mVBOs.push_back(inVBO);
  inVBO->Bind();
  AddVertexAttrib(inAttribLocation, inVertexAttrib);
}

void VAO::SetEBO(const std::shared_ptr<EBO>& inEBO)
{
  const GLBindGuard<GL::EBindingType::EBO> ebo_bind_guard; // First EBO so that the guard is released the latest
  const auto vao_bind_guard = BindGuarded();

  mEBO = inEBO;
  inEBO->Bind();
}

void VAO::AddVertexAttrib(const GL::Id inAttribLocation, const VAOVertexAttrib& inVertexAttrib)
{
  EXPECTS(inVertexAttrib.mNumComponents > 0);

  const auto vao_bind_guard = BindGuarded();

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

  const auto vao_bind_guard = BindGuarded();

  GL::DisableVertexAttribArray(inAttribLocation);
}

const std::shared_ptr<EBO>& VAO::GetEBO() const { return mEBO; }

const std::vector<std::shared_ptr<VBO>>& VAO::GetVBOs() const { return mVBOs; }
}
