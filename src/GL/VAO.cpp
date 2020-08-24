#include <ez/VAO.h>
#include <ez/EBO.h>
#include <ez/GL.h>
#include <ez/GLGuard.h>
#include <ez/Macros.h>
#include <ez/VBO.h>
#include <GL/glew.h>

namespace ez
{
void VAO::AddVBO(const std::shared_ptr<VBO>& inVBO,
    const GL::Id inAttribLocation,
    const VAOVertexAttrib& inVertexAttrib)
{
  const GLBindGuard<GL::EBindingType::VBO> vbo_bind_guard; // First VBO so that the guard is released the latest
  const auto vao_bind_guard = BindGuarded();

  mVBOs.resize(std::max(inAttribLocation + 1ul, mVBOs.size()));
  mVBOs[inAttribLocation] = inVBO;

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
  if (GL::IsFloatingType(inVertexAttrib.mType))
  {
    GL::VertexAttribPointer(inAttribLocation,
        inVertexAttrib.mNumComponents,
        inVertexAttrib.mType,
        inVertexAttrib.mNormalized,
        inVertexAttrib.mStride,
        inVertexAttrib.mOffset);
  }
  else
  {
    if (inVertexAttrib.mNormalized)
      THROW_EXCEPTION("Integer vertex attributes can't be normalized");

    GL::VertexAttribIPointer(inAttribLocation,
        inVertexAttrib.mNumComponents,
        inVertexAttrib.mType,
        inVertexAttrib.mStride,
        inVertexAttrib.mOffset);
  }
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
