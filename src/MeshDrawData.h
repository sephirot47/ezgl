#pragma once

#include "EBO.h"
#include "GLGuard.h"
#include "GLTypeTraits.h"
#include "Mesh.h"
#include "VAO.h"
#include "VBO.h"
#include <memory>

namespace egl
{
class MeshDrawData
{
public:
  using GLGuardType = GLMultiGuard<VAO>;
  using EBOIndexType = Mesh::VertexId;
  static constexpr auto EBOGLIndexType = GLTypeTraits<Mesh::VertexId>::GLType;

  static constexpr GL::Id PositionAttribLocation() { return 0; }
  static constexpr GL::Id NormalAttribLocation() { return 1; }
  static constexpr GL::Id TextureCoordinateAttribLocation() { return 2; }

  MeshDrawData() = default;
  explicit MeshDrawData(const Mesh& inMesh);
  MeshDrawData(const MeshDrawData&) = delete;
  MeshDrawData& operator=(const MeshDrawData&) = delete;
  MeshDrawData(MeshDrawData&&) noexcept = default;
  MeshDrawData& operator=(MeshDrawData&&) = default;
  virtual ~MeshDrawData() = default;

  void Bind() const;
  [[nodiscard]] GLGuardType BindGuarded() const;
  void ComputeFromMesh(const Mesh& inMesh);
  std::size_t GetNumberOfElements() const { return mNumberOfElements; }

  const VAO& GetVAO() const
  {
    EXPECTS(mVAO);
    return *mVAO;
  }

private:
  std::unique_ptr<VAO> mVAO = std::make_unique<VAO>();
  std::size_t mNumberOfElements = 0;
};

}