#pragma once

#include "GL.h"
#include <cstdint>
#include <memory>
#include <vector>

namespace egl
{

class EBO;
class VAOVertexAttrib;
class VBO;

class VAO
{
public:
  VAO();
  VAO(const VAO& inRHS) = delete;
  VAO& operator=(const VAO& inRHS) = delete;
  VAO(VAO&& ioRHS) noexcept;
  VAO& operator=(VAO&& ioRHS) noexcept;
  ~VAO();

  void Bind() const;
  void UnBind() const;
  bool IsBound() const;
  static GL::Id GetBoundGLId();

  void AddVBO(const std::shared_ptr<VBO>& inVBO, const GL::Id inAttribLocation, const VAOVertexAttrib& inVertexAttrib);
  void SetEBO(const std::shared_ptr<EBO>& inEBO);

  void AddVertexAttrib(const GL::Id inAttributeLocation, const VAOVertexAttrib& inVertexAttrib);
  void RemoveVertexAttrib(const GL::Id inAttributeLocation);

  const std::shared_ptr<EBO>& GetEBO() const;
  const std::vector<std::shared_ptr<VBO>>& GetVBOs() const;
  GL::Id GetGLId() const;

private:
  GL::Id mGLId = 0;
  std::shared_ptr<EBO> mEBO;
  std::vector<std::shared_ptr<VBO>> mVBOs;
};
}

#include "VAO.tcc"
