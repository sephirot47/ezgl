#pragma once

#include "EBO.h"
#include "Mesh.h"
#include "VAO.h"
#include "VBO.h"
#include <memory>

namespace egl
{
class DrawableMesh : public Mesh
{
public:
  static constexpr GL::Id PositionAttribLocation() { return 0; }
  static constexpr GL::Id NormalAttribLocation() { return 1; }

  DrawableMesh() = default;
  DrawableMesh(const DrawableMesh&) = delete;
  DrawableMesh& operator=(const DrawableMesh&) = delete;
  DrawableMesh& operator=(DrawableMesh&&) = default;
  DrawableMesh(DrawableMesh&&) = default;
  virtual ~DrawableMesh() = default;

  enum class ENormalsType
  {
    SMOOTH,
    FLAT
  };

  void Bind() const;
  void UpdateVAOs(const DrawableMesh::ENormalsType& inNormalsType = ENormalsType::SMOOTH);

  void Read(const std::filesystem::path& inMeshPath) final;

private:
  VAO mVAO;
};
}