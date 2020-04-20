#include "VAO.h"
#include "EBO.h"
#include "GL.h"
#include "Macros.h"
#include "VBO.h"
#include <GL/glew.h>

namespace egl
{
VAO::VAO() : mGLId(GL::CreateVertexArray())
{
  if (mGLId == 0)
    THROW_EXCEPTION("Error creating VAO");
}

VAO::VAO(VAO&& ioRHS) noexcept
{
  EXPECTS(mGLId == 0);

  std::swap(mGLId, ioRHS.mGLId);

  mEBO = std::move(ioRHS.mEBO);
  mVBOs = std::move(ioRHS.mVBOs);
}

VAO::~VAO()
{
  if (mGLId != 0)
    GL::DeleteVertexArray(mGLId);
}

void VAO::Bind() const { GL::BindVertexArray(mGLId); }
void VAO::UnBind() const { GL::BindVertexArray(0); }
bool VAO::IsBound() const
{
  const auto bound_id = GetBoundGLId();
  return bound_id != 0 && bound_id == mGLId;
}
GL::Id VAO::GetBoundGLId()
{
  const auto bound_id = GL::GetInteger(GL::EBindingType::VERTEX_ARRAY);
  return static_cast<GL::Id>(bound_id);
}

void VAO::AddVBO(const std::shared_ptr<VBO>& inVBO,
    const GL::Id inAttribLocation,
    const VAOVertexAttrib& inVertexAttrib)
{
  EXPECTS(IsBound());

  mVBOs.push_back(inVBO);

  inVBO->Bind();
  AddVertexAttrib(inAttribLocation, inVertexAttrib);
}

void VAO::SetEBO(const std::shared_ptr<EBO>& inEBO)
{
  EXPECTS(IsBound());

  mEBO = inEBO;
  inEBO->Bind();
}

void VAO::AddVertexAttrib(const GL::Id inAttribLocation, const VAOVertexAttrib& inVertexAttrib)
{
  EXPECTS(IsBound());
  EXPECTS(inVertexAttrib.mNumComponents > 0);
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
  EXPECTS(IsBound());
  EXPECTS(inAttribLocation > 0);
  GL::DisableVertexAttribArray(inAttribLocation);
}

const std::shared_ptr<EBO>& VAO::GetEBO() const { return mEBO; }

const std::vector<std::shared_ptr<VBO>>& VAO::GetVBOs() const { return mVBOs; }

GL::Id VAO::GetGLId() const { return mGLId; }
}
