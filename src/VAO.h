#pragma once

#include "GL.h"
#include "GLBindableObject.h"
#include "GLGuard.h"
#include <cstdint>
#include <memory>
#include <vector>

namespace egl
{

class EBO;
class VAOVertexAttrib;
class VBO;

class VAO final : public GLBindableObject<GL::EBindingType::VAO>
{
public:
  using GLGuardType = GLBindGuard<GL::EBindingType::VAO>;

  VAO() = default;
  ~VAO() override = default;
  VAO(VAO&& ioRHS) noexcept = default;

  void AddVBO(const std::shared_ptr<VBO>& inVBO, const GL::Id inAttribLocation, const VAOVertexAttrib& inVertexAttrib);
  void SetEBO(const std::shared_ptr<EBO>& inEBO);

  void AddVertexAttrib(const GL::Id inAttributeLocation, const VAOVertexAttrib& inVertexAttrib);
  void RemoveVertexAttrib(const GL::Id inAttributeLocation);

  const std::shared_ptr<EBO>& GetEBO() const;
  const std::vector<std::shared_ptr<VBO>>& GetVBOs() const;

private:
  std::shared_ptr<EBO> mEBO;
  std::vector<std::shared_ptr<VBO>> mVBOs;
};
}

#include "VAO.tcc"
