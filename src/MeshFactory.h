#pragma once

#include "DrawableMesh.h"
#include "Mesh.h"

namespace egl
{
template <typename TMesh>
class GMeshFactory
{
public:
  GMeshFactory() = delete;

  static TMesh GetCube();
  static TMesh GetSphere(const std::size_t inNumVerticesX, const std::size_t inNumVerticesY);
  static TMesh GetHemisphere(const std::size_t inNumVerticesX, const std::size_t inNumVerticesY);
  static TMesh GetCone(const std::size_t inNumVerticesX);

private:
  static TMesh GetSphere(const std::size_t inNumVerticesX, const std::size_t inNumVerticesY, const bool inIsHemisphere);
  static  void ConsolidateMesh(TMesh& ioMesh, const DrawableMesh::ENormalsType &inNormalsType);
};

using MeshFactory = GMeshFactory<Mesh>;
using DrawableMeshFactory = GMeshFactory<DrawableMesh>;

}

#include "MeshFactory.tcc"