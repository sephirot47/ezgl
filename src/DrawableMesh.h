#pragma once

#include "EBO.h"
#include "GLGuard.h"
#include "Mesh.h"
#include "VAO.h"
#include "VBO.h"
#include <memory>

namespace egl
{
class DrawableMesh : public Mesh
{
public:
  using GLBindGuardType = GLCompositeGuard<VAO>;

  static constexpr GL::Id PositionAttribLocation() { return 0; }
  static constexpr GL::Id NormalAttribLocation() { return 1; }
  static constexpr GL::Id TextureCoordinateAttribLocation() { return 2; }

  DrawableMesh() = default;
  DrawableMesh(const DrawableMesh&) = delete;
  DrawableMesh& operator=(const DrawableMesh&) = delete;
  DrawableMesh(DrawableMesh&&) noexcept = default;
  DrawableMesh& operator=(DrawableMesh&&) = default;
  virtual ~DrawableMesh() = default;

  void Bind() const;
  void UpdateVAOs();

  void Read(const std::filesystem::path& inMeshPath) final;

private:
  std::unique_ptr<VAO> mVAO = std::make_unique<VAO>();
};

}